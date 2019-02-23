#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <fstream>
#include <regex>

#include "node.h"
#include "tree.h"

namespace dom
{
    
    class Interpreter
    {
    private:
        Tree* dtree;
        const static std::regex SELECTOR_CMD_FORMAT;
    public:
        Interpreter(Tree* _dtree);

        bool runScript(std::string& scriptSrc) const;
    
        Node* select(std::string& selectStr) const;
        
        void resolveCmd(std::string& cmd) const;

        void resolveSubCmd(std::string& subCmd, Node* selected) const;
    };

} // namespace dom


#endif // INTERPRETER_H