#ifndef SCRIPT_H
#define SCRIPT_H

#include <iostream>
#include <vector>
#include <regex>

#include "tree.h"
#include "vm.h"

#define SELECTOR        0
#define MULTI_SELECTOR  1
#define OPENING_BRACE   2
#define CLOSING_BRACE   3
#define SEMICOLON       4
#define FUNCTION_CALL   5

namespace dom {
    
    class ScriptRunner
    {
    private:
        Tree* tree;
        VirtualMachine vm;
        std::string src;

        struct RegexGroup
        {
            std::regex rgx;
            unsigned int type;
        };

        struct ScriptToken
        {
            std::string token;
            unsigned int type;
        };

        static std::string getSelectorString(ScriptToken& token);

        RegexGroup regexGroups[6];
        std::vector<ScriptToken> tokens;
        int tokenize();

        int validate();

        std::vector<dom::BytecodeInstruction> instructions;
        int parse();

        static std::vector<BytecodeInstruction> resolveFuncCall(ScriptToken& token, bool multiplied);
    public:
        ScriptRunner(Tree* tree, char* scriptSrc);

        int run();
    };

} // namespace dom

#endif  // SCRIPT_H