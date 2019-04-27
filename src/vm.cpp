#include "vm.h"
#include "util.h"
#include "style.h"

namespace dom {

    void logVMError(const std::string& err)
    {
        Log(COLOR_RED << "VMError: " << err << COLOR_RESET);
    }

    VirtualMachine::VirtualMachine(Tree* tree) : tree(tree)
    {
        loadRoutines();
        selection = nullptr;
        buffer = nullptr;
        flag = false;
    }

    void VirtualMachine::loadRoutines()
    {
        routines.insert(std::make_pair("SEL", &VirtualMachine::routineSEL));
        routines.insert(std::make_pair("PRINT", &VirtualMachine::routinePRINT));
        routines.insert(std::make_pair("SAVE", &VirtualMachine::routineSAVE));

        routines.insert(std::make_pair("ATTRS", &VirtualMachine::routineATTRS));
        routines.insert(std::make_pair("CHILDREN", &VirtualMachine::routineCHILDREN));
        routines.insert(std::make_pair("INNERHTML", &VirtualMachine::routineINNERHTML));
        routines.insert(std::make_pair("PARENT", &VirtualMachine::routinePARENT));

        routines.insert(std::make_pair("IFSEL", &VirtualMachine::routineIFSEL));
        routines.insert(std::make_pair("IFNSEL", &VirtualMachine::routineIFNSEL));
        routines.insert(std::make_pair("SELCLR", &VirtualMachine::routineSELCLR));
        routines.insert(std::make_pair("MSELCLR", &VirtualMachine::routineMSELCLR));
        routines.insert(std::make_pair("ASELCLR", &VirtualMachine::routineASELCLR));
        routines.insert(std::make_pair("FLAGSET", &VirtualMachine::routineFLAGSET));
        routines.insert(std::make_pair("FLAGCLR", &VirtualMachine::routineFLAGCLR));
        routines.insert(std::make_pair("BUFSET", &VirtualMachine::routineBUFSET));
        routines.insert(std::make_pair("BUFLCR", &VirtualMachine::routineBUFCLR));
    }

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
            logVMError("Tree not initialized.");
            return -1;
        }

        for (const Statement& statement : statements)
            execute(statement);

        return 0;
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
        // TODO: Implement optional formatting
        std::string* arg = (std::string*) rawArg;
        std::ofstream fd(*arg);
        fd << "<!DOCTYPE html>" << std::endl;
        fd << tree->toHTML();
        fd.close();
        Log("Tree saved to " << *arg);
    }

    // Dependent
    int VirtualMachine::routineATTRS(void*)
    {
        if (selection == nullptr)
        {
            logVMError("Selection not made for dependent instruction: ATTRS");
            return -1;
        }

        std::function<void(const std::string&, const std::string&)> lambda =
                [](const std::string& key, const std::string& value) {
                    std::cout << key << ": " << value << std::endl;
                };

        selection->forEachAttribute(lambda);
        return 0;
    }

    int VirtualMachine::routineCHILDREN(void*)
    {
        if (selection == nullptr)
        {
            logVMError("Selection not made for dependent instruction: CHILDREN");
            return -1;
        }
        
        std::function<void(const Node* child)> lambda = [](const Node* child) {
            Log("- " << child->toString());
        };

        selection->forEachChild(lambda);
        return 0;
    }

    int VirtualMachine::routineINNERHTML(void*)
    {
        if (selection == nullptr)
        {
            logVMError("Selection not made for dependent instruction: INNERHTML");
            return -1;
        }
        
        auto innerHTML = selection->getInnerHTML();
        if (!util::isBlank(innerHTML))
            Log(innerHTML);

        return 0;
    }

    int VirtualMachine::routinePARENT(void*)
    {
        if (selection == nullptr)
        {
            logVMError("Selection not made for dependent instruction: PARENT");
            return -1;
        }

        auto parent = selection->getParent();
        if (parent == nullptr)
        {
            Log("Root node has no parent.");
            return 0;
        }

        Log(selection->getParent()->toString());
    }

    // Meta
    int VirtualMachine::routineIFSEL(void* rawArg)
    {
        if (selection != nullptr)
        {
            std::vector<Statement>* subroutine = (std::vector<Statement>*) rawArg;
            return execute(*subroutine);
        }

        return 1;
    }

    int VirtualMachine::routineIFNSEL(void* rawArg)
    {
        if (selection == nullptr)
        {
            std::vector<Statement>* subroutine = (std::vector<Statement>*) rawArg;
            return execute(*subroutine);
        }

        return 1;
    }
    
    int VirtualMachine::routineSELCLR(void*)
    {
        selection = nullptr;
        return 0;
    }

    int VirtualMachine::routineMSELCLR(void*)
    {
        mselection.clear();
        return 0;
    }

    int VirtualMachine::routineASELCLR(void*)
    {
        routineSELCLR(nullptr);
        routineMSELCLR(nullptr);
        return 0;
    }

    int VirtualMachine::routineFLAGSET(void*)
    {
        flag = true;
        return 0;
    }

    int VirtualMachine::routineFLAGCLR(void*)
    {
        flag = false;
        return 0;
    }

    int VirtualMachine::routineBUFSET(void* buffer)
    {
        this->buffer = buffer;
        return 0;
    }

    int VirtualMachine::routineBUFCLR(void*)
    {
        this->buffer = nullptr;
        return 0;
    }

} // namespace dom
