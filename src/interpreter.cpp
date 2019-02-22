#include "interpreter.h"
#include "util.h"

namespace dom {

    const std::regex Interpreter::SELECTOR_CMD_FORMAT{R"(\$\(".*"\))"};

    Interpreter::Interpreter(Tree* _dtree) : dtree(_dtree) {}

    void Interpreter::resolveCmd(std::string& cmd) const
    {
        if (cmd == "print") dtree->print();
    }

    void Interpreter::resolveSubCmd(std::string& subCmd, Node* selected) const
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

    Node* Interpreter::select(std::string& cmd) const
    {
        // Check if first function call matches the selector command format
        if (regex_match(cmd.substr(0, cmd.find_first_of(')') + 1), SELECTOR_CMD_FORMAT))
        {
            std::string selector = cmd.substr(3, cmd.find_first_of(')') - 4);
            return dtree->match(selector);
        }
        else
        {
            Log("Invalid syntax: " << cmd);
            return nullptr;
        }   
    }

} // namespace dom
