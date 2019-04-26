#include "vm.h"
#include "util.h"

#include <fstream>

namespace dom {

    const std::regex VirtualMachine::SELECTOR_CMD_FORMAT{R"(\$\(".*"\))"};

    VirtualMachine::VirtualMachine(Tree* tree) : tree(tree) {}

    void VirtualMachine::executeCmd(std::string &cmd) const
    {
        std::vector<std::string> cmds = util::tokenize(cmd);

        if (cmds.at(0) == "print") tree->print();
        else if (cmds.at(0) == "save")
        {
            if (cmds.size() != 2)
            {
                util::logSyntaxError("Usage: save <output-file>");
                return;
            }
            
            std::ofstream fd(cmds.at(1));
            fd << "<!DOCTYPE html>" << std::endl;
            fd << tree->toHTML();
            fd.close();
            Log("Tree saved to " << cmds.at(1));
        }
    }

    void VirtualMachine::executeSubCmd(std::string &subCmd, Node *selected) const
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
        else if (subCmd == "innerhtml")
        {
            auto innerHTML = selected->getInnerHTML();
            if (!util::isBlank(innerHTML))
                Log(innerHTML);
        }

        else Log("Unknown sub-command: " << subCmd);
    }

    Node* VirtualMachine::select(std::string& cmd) const
    {
        // Check if first function call matches the selector command format
        if (regex_match(cmd.substr(0, cmd.find_first_of(')') + 1), SELECTOR_CMD_FORMAT))
        {
            std::string selector = cmd.substr(3, cmd.find_first_of(')') - 4);
            return tree->match(selector);
        }
        else
        {
            Log("Invalid syntax: " << cmd);
            return nullptr;
        }   
    }

} // namespace dom
