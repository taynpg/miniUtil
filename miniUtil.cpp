#include "miniUtil.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

#ifdef OS_MINI_WINDOWS
#include <Windows.h>
static HANDLE hOut = nullptr;
#endif

#ifdef OS_MINI_WINDOWS
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

        if (hOut == nullptr) {
            hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        DWORD written;
        WriteConsoleW(hOut, buffer.data(), wideLen, &written, nullptr);

        return n;
    }
};
#endif

miniUtil::miniUtil()
{
}

miniUtil::~miniUtil()
{
}

std::string miniUtil::Upper(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}
std::string miniUtil::Lower(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

void miniUtil::setU8Output()
{
#ifdef OS_MINI_WINDOWS
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

std::pair<miniErr, std::string> miniUtil::U82Ansi(const std::string& str)
{
#ifdef OS_MINI_WINDOWS
    int wideCharLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (wideCharLen <= 0) {
        return {"Error1", ""};
    }
    std::wstring wideStr(wideCharLen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wideStr[0], wideCharLen);
    int acpLen = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (acpLen <= 0) {
        return {"Error2", ""};
    }
    std::string acpStr(acpLen, '\0');
    WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &acpStr[0], acpLen, nullptr, nullptr);

    acpStr.resize(acpLen - 1);
    return {"", acpStr};
#else
    return {"", str};
#endif
}

std::pair<miniErr, std::string> miniUtil::AutoU8(const std::string& str)
{
    if (!IsU8(str)) {
        return Ansi2U8(str);
    }
    return {"", str};
}

std::pair<miniErr, std::string> miniUtil::Ansi2U8(const std::string& str)
{
#ifdef OS_MINI_WINDOWS
    int wideCharLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (wideCharLen <= 0) {
        return {"Error1", ""};
    }
    std::wstring wideStr(wideCharLen, L'\0');
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wideStr[0], wideCharLen);

    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Len <= 0) {
        return {"Error2", ""};
    }
    std::string utf8Str(utf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Len, nullptr, nullptr);

    utf8Str.resize(utf8Len - 1);
    return {"", utf8Str};
#else
    return {"", str};
#endif
}

bool miniUtil::IsU8(const std::string& str)
{
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(str.data());
    size_t len = str.size();

    for (size_t i = 0; i < len;) {
        uint8_t b = bytes[i];

        // 1-byte: 0xxxxxxx
        if ((b & 0x80) == 0x00) {
            ++i;
            continue;
        }
        // 2-byte: 110xxxxx 10xxxxxx
        else if ((b & 0xE0) == 0xC0) {
            if (i + 1 >= len) {
                return false;
            }
            if ((bytes[i + 1] & 0xC0) != 0x80) {
                return false;
            }
            i += 2;
        }
        // 3-byte: 1110xxxx 10xxxxxx 10xxxxxx
        else if ((b & 0xF0) == 0xE0) {
            if (i + 2 >= len) {
                return false;
            }
            if ((bytes[i + 1] & 0xC0) != 0x80) {
                return false;
            }
            if ((bytes[i + 2] & 0xC0) != 0x80) {
                return false;
            }
            if (b == 0xE0 && bytes[i + 1] < 0xA0) {
                return false;
            }
            i += 3;
        }
        // 4-byte: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        else if ((b & 0xF8) == 0xF0) {
            if (i + 3 >= len) {
                return false;
            }
            if ((bytes[i + 1] & 0xC0) != 0x80) {
                return false;
            }
            if ((bytes[i + 2] & 0xC0) != 0x80) {
                return false;
            }
            if ((bytes[i + 3] & 0xC0) != 0x80) {
                return false;
            }
            if (b == 0xF0 && bytes[i + 1] < 0x90) {
                return false;
            }
            i += 4;
        } else {
            return false;
        }
    }

    return true;
}
