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
        Node* selection;
        std::vector<Node*> mselection;
        void* buffer;
        bool flag;

        typedef int (VirtualMachine::*Routine)(void*);
        std::map<std::string, Routine> routines;
        void loadRoutines();
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

        // Dependent
        int routineATTRS(void*);
        int routineCHILDREN(void*);
        int routineINNERHTML(void*);
        int routinePARENT(void*);

        // Meta
        inline int routineIFSEL(void*);
        inline int routineIFNSEL(void*);
        inline int routineSELCLR(void*);
        inline int routineMSELCLR(void*);
        inline int routineASELCLR(void*);
        inline int routineFLAGSET(void*);
        inline int routineFLAGCLR(void*);
        inline int routineBUFSET(void*);
        inline int routineBUFCLR(void*);
    };

} // namespace dom

#endif