#ifndef MINI_UTIL_H
#define MINI_UTIL_H

#include <string>
#include <vector>

/*
 * 最基本的辅助工具，无需额外依赖。
 * 
 * version: 0.1.0
 * 
*/

class miniUtil
{
public:
    miniUtil();
    ~miniUtil();

public:
    static void setU8Output();
    static std::vector<std::string> split(const std::string& str, const std::string& delim);
};

#endif   // MINI_UTIL_H