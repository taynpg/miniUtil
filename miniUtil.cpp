#include "miniUtil.h"

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

class Utf8ConsoleBuf : public std::streambuf
{
protected:
    int overflow(int c = EOF) override
    {
        if (c != EOF) {
            char ch = static_cast<char>(c);
            xsputn(&ch, 1);
        }
        return c;
    }

    std::streamsize xsputn(const char* s, std::streamsize n) override
    {
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, s, static_cast<int>(n), nullptr, 0);
        if (wideLen <= 0) {
            return 0;
        }

        std::vector<wchar_t> buffer(wideLen);
        MultiByteToWideChar(CP_UTF8, 0, s, static_cast<int>(n), buffer.data(), wideLen);

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD written;
        WriteConsoleW(hOut, buffer.data(), wideLen, &written, nullptr);

        return n;
    }
};

miniUtil::miniUtil()
{
}

miniUtil::~miniUtil()
{
}

void miniUtil::setU8Output()
{
#ifdef _WIN32
    static Utf8ConsoleBuf buf;
    std::cout.rdbuf(&buf);
#else
#endif
}

std::vector<std::string> miniUtil::split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> result;

    if (delim.empty()) {
        result.push_back(str);
        return result;
    }

    std::string::size_type start = 0;
    std::string::size_type pos;

    while ((pos = str.find(delim, start)) != std::string::npos) {
        result.emplace_back(str, start, pos - start);
        start = pos + delim.size();
    }

    result.emplace_back(str, start, str.size() - start);
    return result;
}
