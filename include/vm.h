#ifndef VM_H
#define VM_H

#include <fstream>
#include <regex>

#include "node.h"
#include "tree.h"

namespace dom
{
    class VirtualMachine
    {
    private:
        Tree* tree;
        const static std::regex SELECTOR_CMD_FORMAT;
    public:
        explicit VirtualMachine(Tree* tree);
    
        Node* select(std::string& selectStr) const;
        
        void executeCmd(std::string &cmd) const;

        void executeSubCmd(std::string &subCmd, Node *selected) const;
    };

} // namespace dom


#endif // VM_H