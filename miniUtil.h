#ifndef MINI_UTIL_H
#define MINI_UTIL_H

#include <string>
#include <utility>
#include <vector>

/*
 * 最基本的辅助工具，无需额外依赖。
 *
 * version: 0.3.0
 *
 * 所有 std::pair 返回值的，统一，第一个值是错误字符串，为空正常。
 */

#ifdef _WIN32
#define OS_MINI_WINDOWS
#else
#define OS_MINI_UNIXLIKE
#endif

using miniErr = std::string;
class miniUtil
{
public:
    miniUtil();
    ~miniUtil();

public:
    static void setU8Output();
    static std::vector<std::string> split(const std::string& str, const std::string& delim);
    static std::pair<miniErr, std::string> U82Ansi(const std::string& str);
    static std::pair<miniErr, std::string> Ansi2U8(const std::string& str);
    static std::string Upper(const std::string& str);
    static std::string Lower(const std::string& str);
};

template<typename T>
class miniSingleton
{
public:
    miniSingleton(const miniSingleton&) = delete;
    miniSingleton& operator=(const miniSingleton&) = delete;

    static T& instance()
    {
        // C++11 起：局部静态变量初始化是线程安全的
        static T instance;
        return instance;
    }


private:
    miniSingleton() = default;
    ~miniSingleton() = default;
};

#endif   // MINI_UTIL_H
