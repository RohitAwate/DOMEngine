#include "script.h"
#include "util.h"
#include "style.h"

#include <fstream>

namespace dom
{

    ScriptRunner::ScriptRunner(Tree* tree, char* scriptSrc) : tree(tree), vm(tree)
    {
        std::ifstream fd(scriptSrc);

        std::string line;
        while (std::getline(fd, line))
        {
            cleanSource(line);
            this->src.push_back(line);
        }

        fd.close();
    }

    /*
        Generates a stream of tokens by eliminating whitespaces which
        are then parsed into a vector of ScriptCommand objects.
    */
    int ScriptRunner::parse()
    {
        if (src.empty()) return -1;

        commands = new std::vector<ScriptCommand*>();
        std::string pendingSelector;
        std::string pendingCmd;
        std::vector<std::string>* subCmds;
        bool bracesPaired = true;
        bool semicolonFound = true;

        for (std::string line : src)
        {
            auto tokens = util::tokenizeWhitespace(line);
            
            for (std::string token: tokens)
            {
                // Selector
                if (token[0] == '$')
                {
                    if (!pendingSelector.empty())
                    {
                        util::logSyntaxError("Nested selectors not allowed");
                        return -1;
                    }

                    if (!pendingCmd.empty() && !semicolonFound)
                    {
                        util::logSyntaxError("Semi-colon missing for command: " + pendingCmd);
                        return -1;
                    }

                    pendingSelector = token;
                    subCmds = new std::vector<std::string>();
                }

                // Opening brace
                else if (token[0] == '{')
                {
                    if (!bracesPaired || pendingSelector.empty())
                    {
                        util::logSyntaxError("Stray '{'");
                        return -1;
                    }

                    bracesPaired = false;
                }

                // Closing brace
                else if (token[0] == '}')
                {
                    if (bracesPaired || pendingSelector.empty())
                    {
                        util::logSyntaxError("Stray '}'");
                        return -1;
                    }

                    commands->push_back(new ScriptCommand{pendingSelector, subCmds});
                    pendingSelector.clear();
                    bracesPaired = true;
                }

                // Semi-colon
                else if (token[0] == ';')
                {
                    if (semicolonFound)
                    {
                        util::logSyntaxError("Stray ';'");
                        return -1;
                    }
                    else
                    {
                        semicolonFound = true;
                        if (pendingSelector.empty())
                            commands->push_back(new ScriptCommand{pendingCmd, nullptr});
                        else
                            subCmds->push_back(pendingCmd);
                    }
                }

                // Command
                else
                {
                    if (!semicolonFound && !pendingCmd.empty())
                    {
                        util::logSyntaxError("Semi-colon missing for command: " + pendingCmd);
                        return -1;
                    }

                    // Check if command ends in semicolon
                    if (token[token.length() - 1] != ';')
                    {
                        semicolonFound = false;
                        pendingCmd = token;
                        continue;
                    }

                    // Remove semi-colon
                    auto tokenTrunc = token.substr(0, token.length() - 1);
                    if (pendingSelector.empty())
                        commands->push_back(new ScriptCommand{tokenTrunc, nullptr});
                    else
                        subCmds->push_back(tokenTrunc);
                }
            }
        }

        if (!semicolonFound)
        {
            util::logSyntaxError("Semi-colon missing for command: " + pendingCmd);
            return -1;
        }

        if (!bracesPaired)
        {
            util::logSyntaxError("Braces not paired for selector: " + pendingSelector);
            return -1;
        }

        if (!pendingSelector.empty())
        {
            util::logSyntaxError("Selector block body not found for selector: " + pendingSelector);
            return -1;
        }
        
        return 1;
    }

    /*
        Inserts spaces at appropriate locations in the string
        to make it easier for tokenization.

        Spaces are inserted:
        - before $
        - after ; and )
        - before and after { and }
    */
    void ScriptRunner::cleanSource(std::string& line)
    {
        int curr, len;
        char space = 32;

        curr = 0;
        len = line.length();
        while (curr < len)
        {
            // Before
            if (line[curr] == '$' && curr-1 >= 0)
            {
                line.insert(curr, 1, space);
                len++;
                curr += 2;
                continue;
            }

            // After
            else if (line[curr] == ';' || line[curr] == ')')
            {
                line.insert(curr+1, 1, space);
                len++;
                curr += 2;
                continue;
            }
            
            // Both
            else if (line[curr] == '{' || line[curr] == '}')
            {
                if (curr-1 >= 0) 
                {
                    line.insert(curr, 1, space);
                    line.insert(curr+2, 1, space);
                    len += 2;
                    curr += 2;
                }
                else
                {
                    line.insert(curr+1, 1, space);
                    len++;
                    curr += 2;
                }
                continue;
            }
            curr++;
        }
    }

    int ScriptRunner::run()
    {
        auto res = parse();
        if (res != 1) return res;

        for (auto scriptCmd : *commands)
        {
            Log("> " << scriptCmd->cmd);
            if (scriptCmd->subCmds)
            {
                auto selected = vm.select(scriptCmd->cmd);
                if (selected)
                {
                    Log(TEXT_BOLD << "$ " << selected->toString() << TEXT_RESET);
                    for (auto subCmd : *scriptCmd->subCmds)
                    {
                        Log(".. > " << subCmd << TEXT_RESET);
                        vm.executeSubCmd(subCmd, selected);
                    }
                }
                else Log("No match found.");
            }
            else
            {
                vm.executeCmd(scriptCmd->cmd);
            }

            Log("────────────────────────");
        }

        return 1;
    }

} // namespace dom
