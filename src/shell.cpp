#include "shell.h"
#include "util.h"

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

        if (cmd == "exit")
            break;
            
        if (cmd == "print")
        {
            dtree->print();
            continue;
        }

        resolveCmd(cmd);
    }
}

void Shell::resolveCmd(std::string& cmd)
{
    if (cmd[0] == '$')
    {
        // Check if first function call matches the selector command format
        if (regex_match(cmd.substr(0, cmd.find_first_of(')') + 1), SELECTOR_CMD_FORMAT))
        {
            std::string selector = cmd.substr(3, cmd.find_first_of(')') - 4);
            auto matchedNode = dtree->match(selector);
            if (matchedNode) Log(matchedNode->toString());
            else Log("No match found: " << selector);
        }
        else Log("Invalid syntax: " << cmd);
    }
    else Log("Unknown command: " << cmd);
}

const std::regex Shell::SELECTOR_CMD_FORMAT{R"(\$\(".*"\))"};

} // namespace dom
