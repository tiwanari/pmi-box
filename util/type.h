#pragma once
#include <sstream>

namespace util {
template <typename T>
inline bool is(const std::string& s)
{
    std::istringstream iss(s);
    T x;
    char c;
    return iss >> x && !(iss >> c);
}
}   // namespace util
