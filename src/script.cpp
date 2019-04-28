#include <fstream>

#include "script.h"
#include "util.h"
#include "style.h"

namespace dom {

    /*
        Returns a new string similar to the source string without any spaces, tabs and newlines.
        This function does not alter whitespaces present inside string literals.
    */
    std::string removeWhitespace(std::string& source)
    {
        bool insideStr = false;

        std::string cleaned;
        for (unsigned int i = 0; i < source.length(); i++) {
            if (source[i] == '\"') {
                insideStr = !insideStr;
                cleaned += source[i];
                continue;
            }

            if ((source[i] != ' ' && source[i] != '\n' && source[i] != '\t') || insideStr) {
                cleaned += source[i];
            }
        }

        return cleaned;
    }

    std::string ScriptRunner::getSelectorString(ScriptToken& token)
    {
        std::string selector;
        int len = token.token.length();
        switch (token.type)
        {
            case SELECTOR:
                selector = token.token.substr(3, len - 5);
                break;
            case MULTI_SELECTOR:
                selector = token.token.substr(4, len - 6);
                break;
        }

        return selector;
    }

    std::vector<BytecodeInstruction> ScriptRunner::resolveFuncCall(ScriptToken& token, bool multiplied)
    {
        std::string& call = token.token;
        std::vector<BytecodeInstruction> subroutine;

        if (call == "print()")
        {
            subroutine.push_back({PRINT, nullptr});
            return subroutine;
        }
        
        if (call == "attrs()")
        {
            subroutine.push_back({multiplied ? MATTRS : ATTRS, nullptr});
            return subroutine;
        }
        
        if (call == "children()")
        {
            subroutine.push_back({multiplied ? MCHILDREN : CHILDREN, nullptr});
            return subroutine;
        }
        
        if (call == "innerhtml()")
        {
            subroutine.push_back({multiplied ? MINNERHTML : INNERHTML, nullptr});
            return subroutine;
        }
        
        if (call == "parent()")
        {
            subroutine.push_back({multiplied ? MPARENT : PARENT, nullptr});
            return subroutine;
        }
        
        if (util::startsWith(call, "save"))
        {
            std::vector<std::string> tokens = util::tokenize(call.substr(5, call.length() - 6), ',', true);
            if (tokens.size() != 2)
            {
                util::logSyntaxError(
                    "Invalid number of arguments for save()\nUsage: save(string, bool).\n"
                    "string filepath: path of file in which to save the HTML representation.\n"
                    "bool format: flag which can turn on/off formatting"
                );

                return subroutine;
            }

            std::string filepath = tokens[0].substr(1, tokens[0].length() - 2);
            subroutine.push_back(BytecodeInstruction{tokens[1] == "true" ? FLAGSET : FLAGCLR, nullptr});
            subroutine.push_back(BytecodeInstruction{SAVE, (void*)(&filepath)});
            return subroutine;
        }
    }

    ScriptRunner::ScriptRunner(Tree* tree, char* scriptSrc) : tree(tree), vm(tree)
    {
        std::ifstream fd(scriptSrc);

        std::string line;
        while (std::getline(fd, line))
            this->src += line;

        fd.close();
    }

    /*
        Scans the source string and uses a set of RegexGroups to generate a
        stream of ScriptTokens which can be used by the parser.
    */
    int ScriptRunner::tokenize()
    {
        if (tokens.size() > 0) return 1;     // for idempotent behaviour

        regexGroups[0] = RegexGroup{std::regex("\\$\\(\"[a-zA-Z0-9_@#\\.\\-]*\"\\)"), SELECTOR };
        regexGroups[1] = RegexGroup{std::regex("\\$!\\(\"[a-zA-Z0-9_@#\\.\\-]*\"\\)"), MULTI_SELECTOR };
        regexGroups[2] = RegexGroup{std::regex("\\{"), OPENING_BRACE };
        regexGroups[3] = RegexGroup{std::regex("\\}"), CLOSING_BRACE };
        regexGroups[4] = RegexGroup{std::regex(";"), SEMICOLON };
        regexGroups[5] = RegexGroup{std::regex("[a-zA-Z]+\\([a-zA-Z\",\\s\\.]*\\)"), FUNCTION_CALL };

        auto tokenSrc = removeWhitespace(src);
        std::smatch match;

        bool deadIteration = false;
        while (tokenSrc.length() > 0)
        {
            deadIteration = true;

            for (const RegexGroup& group : regexGroups)
            {
                if (std::regex_search(tokenSrc, match, group.rgx ,std::regex_constants::match_continuous))
                {
                    tokens.push_back(ScriptToken{match.str(0), group.type});
                    tokenSrc = match.suffix().str();
                    deadIteration = false;
                    break;
                }
            }

            if (deadIteration)
            {
                util::logSyntaxError("Cannot parse further:\n" + tokenSrc + "\n");
                return -1;
            }
        }

        return 0;
    }

    // Validates the grammar for the stream of ScriptTokens generated by the tokenizer.
    int ScriptRunner::validate()
    {
        if (tokens.size() == 0) return -1;      // if parser is not called beforehand

        std::string seq = "";
        bool expectOpening = false;
        for (ScriptToken& token : tokens)
            seq += std::to_string(token.type);

        std::regex selBlock("[01]2(54)*3");
        std::regex funcCall("54");
        std::smatch match;

        while (seq.length() > 0)
        {
            if (std::regex_search(seq, match, selBlock, std::regex_constants::match_continuous))
                seq = match.suffix().str();
            else if (std::regex_search(seq, match, funcCall, std::regex_constants::match_continuous))
                seq = match.suffix().str();
            else
            {
                util::logSyntaxError("Invalid syntax.");
                // TODO: Error reporting
                return -1;
            }
        }

        return 0;
    }

    int ScriptRunner::parse()
    {
        std::string selector;
        std::vector<BytecodeInstruction> subroutine;
        std::vector<BytecodeInstruction> funcInstructions;

        int insideSelector = -1;
        for (int i = 0; i < tokens.size(); i++)
        {
            ScriptToken& token = tokens[i];
            switch (token.type)
            {
                case SELECTOR:
                    selector = getSelectorString(token);
                    instructions.push_back({SEL, (void*)(&selector)});
                    insideSelector = SELECTOR;
                    break;
                case MULTI_SELECTOR:
                    selector = getSelectorString(token);
                    instructions.push_back({MSEL, (void*)(&selector)});
                    insideSelector = MULTI_SELECTOR;
                    break;
                case FUNCTION_CALL:
                    funcInstructions = resolveFuncCall(token, insideSelector == MULTI_SELECTOR);

                    if (insideSelector == -1)
                    {
                        for (auto instr : funcInstructions)
                            instructions.push_back(instr);
                    }
                    else
                    {
                        for (auto instr : funcInstructions)
                            subroutine.push_back(instr);
                    }
                    break;
                case CLOSING_BRACE:
                    subroutine.push_back({insideSelector == SELECTOR ? SELCLR : MSELCLR, nullptr});
                    instructions.push_back({insideSelector == SELECTOR ? IFSEL : IFMSEL, (void*)(&subroutine)});
                    insideSelector = -1;
                    break;
            }
        }
    }

    // Executes the VM Statements generated by the parser in the VirtualMachine.
    int ScriptRunner::run()
    {
        tokenize();
        validate();
        parse();

        vm.execute(instructions);
    }

} // namespace dom
