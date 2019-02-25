#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <map>

#define Log(x) std::cout << x << std::endl

namespace util {

    void logSyntaxError(std::string);
    
    std::vector<std::string> tokenize(std::string in, char delim, bool stringAware = false);

    std::vector<std::string> tokenizeWhitespace(std::string in);

    std::vector<std::string> tokenizeTag(std::string in);

    void readLine(std::string& line);

    bool isBlank(std::string& str);

    template <typename K, typename V>
    V mapGet(std::map<K, V>& m, K key)
    {
        auto itr = m.find(key);
        if (itr != m.end()) return itr->second;
        else return "";
    }

    template <typename T>
    bool vectorContains(std::vector<T>& vec, T key)
    {
        for (auto k : vec)
            if (k == key) return true;

        return false;
    }

} // namespace util

#endif  // UTIL_H