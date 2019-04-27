
#include <vm.h>

namespace dom {

    VirtualMachine::VirtualMachine(Tree* tree) : tree(tree)
    {
        loadRoutines();
    }

    void VirtualMachine::loadRoutines()
    {
        std::vector<std::string> cmds = util::tokenize(cmd);

    int VirtualMachine::execute(const Statement &statement)
    {
        if (tree == nullptr)
        {
            Log("VMError: Tree not initialized.");
            return -1;
        }

        Routine routine = routines[statement.instr];
        (this->*routine)(statement.arg);

        return 0;
    }

    int VirtualMachine::execute(const std::vector<Statement> &statements)
    {
        if (tree == nullptr)
        {
            Log("VMError: Tree not initialized.");
            return -1;
        }

        for (const Statement& statement : statements)
            execute(statement);

        return 0;
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

    // VM Routines
    // Independent
    int VirtualMachine::routineSEL(void* rawArg)
    {
        std::string* arg = (std::string*) rawArg;
        selection = tree->match(*arg);
        return 0;
    }

    int VirtualMachine::routinePRINT(void*)
    {
        tree->print();
        return 0;
    }

    int VirtualMachine::routineSAVE(void* rawArg)
    {
        std::string* arg = (std::string*) rawArg;
        std::ofstream fd(*arg);
        fd << "<!DOCTYPE html>" << std::endl;
        fd << tree->toHTML();
        fd.close();
        Log("Tree saved to " << *arg);
    }

} // namespace dom
