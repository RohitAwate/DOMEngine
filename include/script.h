#ifndef SCRIPT_H
#define SCRIPT_H

#include <iostream>
#include <vector>

namespace dom
{
    
    class ScriptRunner
    {
    private:
        std::vector<std::string> src;
    public:
        ScriptRunner(char* scriptSrc);

        int run();
    };

} // namespace dom


#endif  // SCRIPT_H