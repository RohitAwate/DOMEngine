#include "util.h"
#include "style.h"

namespace util {
    
    std::vector<std::string> tokenize(std::string in, char delim, bool stringAware)
    {
        int start = 0;
        std::vector<std::string> tokens;
        bool quotesPaired = true;

        for (int i = 0; i < in.length(); i++)
        {
            if (in[i] == '\"' && stringAware)
            {
                quotesPaired = !quotesPaired;
            }

            else if (in[i] == delim && quotesPaired)
            {
                tokens.push_back(in.substr(start, i-start));
                start = i+1;
            }
        }

        tokens.push_back(in.substr(start, start-in.length()));

        return tokens;
    }

    bool isBlank(std::string& str)
    {
        return str.find_first_not_of(' ') == std::string::npos;
    }

    bool startsWith(const std::string& str, const std::string& sub)
    {
        int subLen = sub.length();
        return str.substr(0, subLen) == sub;
    }

    bool endsWith(const std::string& str, const std::string& sub)
    {
        int subLen = sub.length();
        return str.substr(str.length() - subLen, subLen) == sub;
    }

    std::vector<std::string> tokenizeWhitespace(std::string in)
    {
        std::vector<std::string> tokens;

        int curr = 0;
        int len = in.length();
        int start;
        std::string token;
        while (curr < len)
        {
            while (curr < len && std::isspace(in[curr])) curr++;
            start = curr;
            
            while (curr < len && !std::isspace(in[curr])) curr++;
            token = in.substr(start, curr-start);
            if (!isBlank(token))
                tokens.push_back(token);
        }

        return tokens;
    }

    std::vector<std::string> tokenizeHTML(std::vector<std::string>& src)
    {
        std::vector<std::string> tokens;

        int curr, len;
        std::string token;
        bool tagComplete = true;

        for (std::string line : src)
        {
            len = line.length();
            curr = 0;

            while (curr < len)
            {
                // If <, set tagComplete to false.
                // Add everything to token until >.
                if (line[curr] == '<')
                {
                    tagComplete = false;
                    token.append(1, line[curr]);
                }

                // If >, set tagComplete to true.
                // Add token to vector.
                // Clear token.
                else if (line[curr] == '>')
                {
                    token.append(1, line[curr]);
                    tagComplete = true;
                    tokens.push_back(token);
                    token.clear();
                }

                // Handle whitespace based on whether tagComplete is set or not
                else if (std::isspace(line[curr]))
                {
                    if (!tagComplete)
                        token.append(1, line[curr]);
                }
                
                // Handle other characters on the basis of tagComplete
                else
                {
                    // For tagname, attributes, etc
                    if (!tagComplete)
                        token.append(1, line[curr]);

                    // For the tag content
                    else
                    {
                        token.clear();
                        // Add everything to a separate token until < is found.
                        while (curr < len && line[curr] != '<')
                            token.append(1, line[curr++]);

                        tokens.push_back(token);
                        token.clear();
                        continue;
                    }
                }
                
                curr++;
            }
        }

        return tokens;
    }

    void readLine(std::string& line)
    {
        std::getline(std::cin >> std::ws, line);
    }

    void logSyntaxError(std::string msg)
    {
        Log(COLOR_RED << "[Syntax Error] " << COLOR_RESET << msg);
    }

} // namespace util
