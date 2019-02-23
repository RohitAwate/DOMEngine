#include "script.h"
#include "util.h"

#include <fstream>

namespace dom
{

    ScriptRunner::ScriptRunner(char* scriptSrc)
    {
        std::ifstream fd(scriptSrc);

        std::string line;
        while (std::getline(fd, line)) this->src.push_back(line);

        fd.close();
    }
    
    int ScriptRunner::run()
    {
        for (auto line: src) Log(line);
        return 1;
    }


} // namespace dom
