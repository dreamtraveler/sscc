#ifndef __GX_PATH_H__
#define __GX_PATH_H__

#include <string>
#include <stack>
#include "platform.h"

#ifdef WIN32
#include <wtypes.h>
#else
#include <sys/types.h>
#include <dirent.h>
#endif

GX_NS_BEGIN
class PathIterator;

class Path {
public:
    typedef PathIterator iterator;
public:
    Path() ;
    Path(const char *x) ;
    Path(const std::string &x) ;
    Path(const Path &x) ;

    operator const char*() const {
        return _path.c_str();
    }

    operator const std::string&() const {
        return _path;
    }

    const char *c_str() const {
        return _path.c_str();
    }

    size_t size() const {
        return _path.size();
    }

    Path &operator=(const char *x) ;
    Path &operator=(const std::string &x) ;
    Path &operator=(const Path &x) ;
    Path &operator=(Path &&x) ;

    Path operator+(const char *x) const ;
    Path operator+(const std::string &x) const ;
    Path operator+(const Path &x) const ;

    void normalize() ;
    std::string filename() const ;
    std::string basename() const ;
    std::string extension() const ;
    void extension(const char *ext) ;
    Path directory() const ;
    bool is_absolute() const ;
    bool operator==(const Path &x) const {
        return _path == x._path;
    }
    bool operator!=(const Path &x) const {
        return _path != x._path;
    }
    bool empty() const {
        return _path.empty();
    }
    void clear() ;

    iterator begin() const ;
    iterator end() const ;

    Path realpath() const ;
    static Path pwd() ;
private:
    std::string _path;
};

class PathIterator {
private:
    struct Directory {
        Directory(const Path &path) ;
        ~Directory() ;

#ifdef WIN32
        HANDLE _dir;
        WIN32_FIND_DATA _data;
#else
        DIR *_dir;
#endif
        Path _path;
    };
public:
    PathIterator(const Path *path) ;
    PathIterator(const PathIterator &x) 
    : _stack(x._stack)
    { }
    const Path &operator*() {
        return _path;
    }
    PathIterator& operator++() ;
    PathIterator operator++(int) {
        PathIterator tmp(*this);
        ++*this;
        return tmp;
    }
    bool operator==(const PathIterator &x) {
        return _path == x._path;
    }
    bool operator!=(const PathIterator &x) {
        return _path != x._path;
    }
private:
    std::stack<Directory> _stack;
    Path _path;
};

GX_NS_END
#endif