#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <map>

#define Log(x) std::cout << x << std::endl

namespace util {
    
std::vector<std::string> tokenize(std::string in, char delim);

void readLine(std::string& line);

template <typename K, typename V>
V mapGet(std::map<K, V>& m, K key)
{
    auto itr = m.find(key);
    if (itr != m.end()) return itr->second;
    else return "";
}

} // namespace util

#endif  // UTIL_H