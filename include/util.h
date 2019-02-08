#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>

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

template <typename K, typename V>
V mapGet(std::map<K, V>& m, K key)
{
    auto itr = m.find(key);
    if (itr != m.end()) return itr->second;
    else return "";
}

} // namespace util

#endif  // UTIL_H