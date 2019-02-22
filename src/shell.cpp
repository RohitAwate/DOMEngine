#include "shell.h"
#include "util.h"
#include "style.h"

#ifndef APP_VERSION
    #define APP_VERSION "(Version not defined. Please use CMake to build the project.)"
#endif

namespace dom {

    Shell::Shell(Tree* _dtree) : dtree(_dtree){}

    void Shell::start()
    {
        if (dtree == nullptr || !dtree->isBuilt())
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
                
            if (cmd == "print")
            {
                dtree->print();
                continue;
            }

            resolveCmd(cmd);
        }
    }

    void Shell::resolveCmd(std::string& cmd) const
    {
        if (cmd[0] == '$')
        {
            // Check if first function call matches the selector command format
            if (regex_match(cmd.substr(0, cmd.find_first_of(')') + 1), SELECTOR_CMD_FORMAT))
            {
                std::string selector = cmd.substr(3, cmd.find_first_of(')') - 4);
                auto matchedNode = dtree->match(selector);
                if (matchedNode) startSubCmdLoop(matchedNode);
                else Log("No match found: " << selector);
            }
            else Log("Invalid syntax: " << cmd);
        }
        else Log("Unknown command: " << cmd);
    }

    const std::regex Shell::SELECTOR_CMD_FORMAT{R"(\$\(".*"\))"};

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

            resolveSubCmd(subCmd, selected);
        }
    }

    void Shell::resolveSubCmd(std::string& subCmd, Node* selected) const
    {
        if (subCmd == "parent")
        {
            auto parent = selected->getParent();
            if (parent == nullptr)
            {
                Log("Root node has no parent.");
                return;
            }

            Log(selected->getParent()->toString());
        }
        else if (subCmd == "children")
        {
            std::function<void(const Node* child)> lambda = [](const Node* child) {
                Log("- " << child->toString());
            };

            selected->forEachChild(lambda);
        }
        else if (subCmd == "attrs")
        {
            std::function<void(const std::string&, const std::string&)> lambda = 
                [](const std::string& key, const std::string& value) {
                    std::cout << key << ": " << value << std::endl;
                };

            selected->forEachAttribute(lambda);
        }
        else Log("Unknown sub-command: " << subCmd);
    }

} // namespace dom
