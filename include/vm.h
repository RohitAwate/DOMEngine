#ifndef VM_H
#define VM_H

#include <fstream>
#include <map>

#include "node.h"
#include "tree.h"

namespace dom  {

    struct Statement
    {
        std::string instr;
        void* arg;
    };

    class VirtualMachine
    {
    private:
        Tree* tree;
        const static std::regex SELECTOR_CMD_FORMAT;
    public:
        explicit VirtualMachine(Tree* tree);

        int execute(const Statement& statement);

        int execute(const std::vector<Statement>& statements);
    private:
        // VM Routines
        // Independent
        int routineSEL(void*);
        int routinePRINT(void*);
        int routineSAVE(void*);
    };

} // namespace dom

#endif