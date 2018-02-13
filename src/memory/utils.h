#ifndef __GX_UTILS_H__
#define __GX_UTILS_H__

#include <vector>
#include <string>
#include <memory>
#include "data.h"
#include "platform.h"

GX_NS_BEGIN
std::string ltrim(const std::string &str) ;
std::string rtrim(const std::string &str) ;
std::string trim(const std::string &str) ;
std::vector<std::string> split(const std::string &str, int c) ;

#ifdef WIN32
#include <windows.h>
std::shared_ptr<Data> char2wchar(const char *str) ;
std::shared_ptr<Data> wchar2char(const wchar_t *str) ;
#endif
GX_NS_END

#endif 