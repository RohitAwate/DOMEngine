#ifndef VM_H
#define VM_H

#include "node.h"
#include "tree.h"

// Instruction op-codes
const unsigned int SEL = 0;
const unsigned int MSEL = 1;
const unsigned int PRINT = 2;
const unsigned int SAVE = 3;

const unsigned int ATTRS = 4;
const unsigned int CHILDREN = 5;
const unsigned int INNERHTML = 6;
const unsigned int PARENT = 7;

const unsigned int MATTRS = 8;
const unsigned int MCHILDREN = 9;
const unsigned int MINNERHTML = 10;
const unsigned int MPARENT = 11;

const unsigned int IFSEL = 12;
const unsigned int IFNSEL = 13;
const unsigned int IFMSEL = 14;
const unsigned int IFNMSEL = 15;
const unsigned int SELCLR = 16;
const unsigned int MSELCLR = 17;
const unsigned int ASELCLR = 18;
const unsigned int FLAGSET = 19;
const unsigned int FLAGCLR = 20;
const unsigned int BUFSET = 21;
const unsigned int BUFCLR = 22;

namespace dom  {

    struct BytecodeInstruction
    {
        unsigned int opcode;
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
        Routine routines[23];
        void loadRoutines();
    public:
        explicit VirtualMachine(Tree* tree);

        int execute(const BytecodeInstruction& instruction);

        int execute(const std::vector<BytecodeInstruction>& subroutine);
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