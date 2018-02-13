#ifndef __GX_DATA_H__
#define __GX_DATA_H__

#include <cstdlib>
#include "platform.h"

GX_NS_BEGIN

class Data {
public:
    Data() : _ptr(), _size() { }
    Data(size_t size) {
        _ptr = (char*)std::malloc(size + 1);
        _size = size;
        _ptr[size] = '\0';
    }
    Data(const Data&) = delete;
    Data(Data &&x) : _ptr(), _size() {
        swap(x);
    }
    ~Data() {
        if (_ptr) {
            std::free(_ptr);
        }
    }
    void swap(Data &x) {
        std::swap(_ptr, x._ptr);
        std::swap(_size, x._size);
    }
    Data &operator=(const Data&) = delete;
    Data &operator=(Data &&x) {
        swap(x);
        return *this;
    }
    char *data() const {
        return _ptr;
    }
    size_t size() const {
        return _size;
    }
protected:
    char *_ptr;
    size_t _size;
};
GX_NS_END

#endif 