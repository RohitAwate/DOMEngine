#include "util.h"

namespace util {
    
    std::vector<std::string> tokenize(std::string in, char delim)
    {
        int start = 0;
        std::vector<std::string> tokens;

        for (int i = 0; i < in.length(); i++)
        {
            if (in[i] == delim)
            {
                tokens.push_back(in.substr(start, i-start));
                start = i+1;
            }
        }

        tokens.push_back(in.substr(start, start-in.length()));

        return tokens;
    }

    void readLine(std::string& line)
    {
        std::cin >> line;
    }

} // namespace util
