#include "shell.h"
#include "util.h"
#include "style.h"

#ifndef APP_VERSION
    #define APP_VERSION "(Version not defined. Please use CMake to build the project.)"
#endif

namespace dom {

    Shell::Shell(Tree* tree) : tree(tree)
    {
        this->interpreter = new Interpreter(tree);
    }

    void Shell::start()
    {
        if (tree == nullptr || !tree->isBuilt())
        {
            Log("DOM Tree not built.");
            return;
        }

        Log("DOM Manipulation Engine " << APP_VERSION);
        Log("(C) 2019 Rohit Awate");

        std::string cmd;
        while (true)
        {
            std::cout << "> ";
            util::readLine(cmd);

            if (cmd.empty()) continue;
            if (cmd == "exit") break;
                
            if (cmd[0] == '$')
            {
                Node* matchedNode = interpreter->select(cmd);
                if (matchedNode) startSubCmdLoop(matchedNode);
                else Log("No match found.");
            }
            else interpreter->resolveCmd(cmd);
        }
    }

    void Shell::startSubCmdLoop(Node* selected) const
    {
        Log(TEXT_BOLD << "$ " << selected->toString() << TEXT_RESET);
        std::string subCmd;

        while (true)
        {
            std::cout << ".. > ";
            util::readLine(subCmd);

            if (subCmd.empty()) continue;
            if (subCmd == "return") return;
            if (subCmd == "exit") std::exit(0);

            interpreter->resolveSubCmd(subCmd, selected);
        }
    }

} // namespace dom
