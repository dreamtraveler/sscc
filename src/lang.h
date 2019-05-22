#ifndef __LANG_H__
#define __LANG_H__

#include <sstream>
#include <cstdio>
#include "unistr.h"
#include "token.h"
#include "printer.h"
#include "symtab.h"

class Language {
public:
    Language(const char *name);
    virtual ~Language() { }
    const char *name() const {
        return _name;
    }

public:
    static Language *get(const char *name);
    template <typename _T>
    static Language *reg() {
        return reg(std::make_shared<_T>());
    }

    std::stringstream &head() {
        return _head;
    }
    std::stringstream &tail() {
        return _tail;
    }
	std::string &filename() {
		return _filename;
	}
	std::string dir() {
		return _dir;
	}


	void set_filename(const char* str) {
		std::string filestr(str);
#if defined(WIN32) || defined(_WIN32)
		int pos_begin = filestr.find_last_of('\\') + 1;
#else
		int pos_begin = filestr.find_last_of('/') + 1;
#endif
		int sz = filestr.size() - 4 - pos_begin;
		_filename = filestr.substr(pos_begin, sz);
		_dir = filestr.substr(0, pos_begin);
	}

    virtual void option(const char *name, std::shared_ptr<Token> value) = 0;
	virtual void print(SymbolTable &symbols, FILE *file) = 0;
private:
    static Language *reg(std::shared_ptr<Language> lang);
	std::string _filename;
	std::string _dir;

protected:
    const char *_name;
    std::stringstream _head;
    std::stringstream _tail;
};


#endif
