#include "utils.h"

GX_NS_BEGIN
std::string ltrim(const std::string &str) {
    const char *p = (char*)str.c_str();
    const char *tail = p + str.size() - 1;

    while (p <= tail) {
        if (*p != ' ' && *p != '\t') {
            break;
        }
        p++;
    }

    return std::string(p, tail - p + 1);
}

std::string rtrim(const std::string &str) {
    const char *p = (char*)str.c_str();
    const char *tail = p + str.size() - 1;

    while (p <= tail) {
        if (*tail != ' ' && *tail != '\t') {
            break;
        }
        tail++;
    }

    return std::string(p, tail - p + 1);
}

std::string trim(const std::string &str) {
    const char *p = (char*)str.c_str();
    const char *tail = p + str.size() - 1;

    while (p <= tail) {
        if (*p != ' ' && *p != '\t') {
            break;
        }
        p++;
    }

    while (p <= tail) {
        if (*tail != ' ' && *tail != '\t') {
            break;
        }
        tail++;
    }

    return std::string(p, tail - p + 1);
}

std::vector<std::string> split(const std::string &str, int c) {
    size_t pos = 0;
    std::vector<std::string> result;
    while (pos < str.size()) {
        size_t n = str.find(c, pos);
        if (n == std::string::npos) {
            result.emplace_back(str.substr(pos));
            break;
        }
        if (pos == n) {
            result.emplace_back();
        }
        else {
            result.emplace_back(str.substr(pos, n - pos));
        }
        pos = n + 1;
    }
    return result;
}

#ifdef WIN32
std::shared_ptr<Data> char2wchar(const char *str) {
    size_t len = strlen(str);
    size_t n = MultiByteToWideChar(CP_ACP, 0, str, len, nullptr, 0);
	std::shared_ptr<Data> buf = std::make_shared<Data>(sizeof(wchar_t)* n + 1);
    MultiByteToWideChar(CP_ACP, 0, str, len, (wchar_t*)buf->data(), n);
    *(buf->data() + (buf->size() - 1)) = '\0';
    return buf;
}

std::shared_ptr<Data> wchar2char(const wchar_t *str) {
    size_t len = wcslen(str);
    size_t n = WideCharToMultiByte(0, 0, str, len, nullptr, 0, nullptr, nullptr);
	std::shared_ptr<Data> buf = std::make_shared<Data>(n);
    WideCharToMultiByte(0, 0, str, len, buf->data(), n, nullptr, nullptr);
    return buf;
}
#endif

GX_NS_END