#ifndef SCRIPT_H
#define SCRIPT_H

#include <iostream>
#include <vector>

#include "tree.h"
#include "interpreter.h"

namespace dom
{
    
    class ScriptRunner
    {
    private:
        Tree* dtree;
        Interpreter interpreter;
        std::vector<std::string> src;

        struct ScriptCommand
        {
            std::string cmd;
            std::vector<std::string>* subCmds;
        };

        std::vector<ScriptCommand*>* commands;

        int parse();

        void cleanSource(std::string& line);
    public:
        ScriptRunner(Tree* _dtree, char* scriptSrc);

        int run();
    };

} // namespace dom


#endif  // SCRIPT_H