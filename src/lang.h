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
	std::string filename;

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
	void set_filename(const char* str) {
		std::string filestr(str);
		int pos_begin = filestr.find_last_of('\\') + 1;
		int sz = filestr.size() - 4 - pos_begin;
		filename = filestr.substr(pos_begin, sz);
	}

    virtual void option(const char *name, std::shared_ptr<Token> value) = 0;
	virtual void print(SymbolTable &symbols, FILE *file) = 0;
private:
    static Language *reg(std::shared_ptr<Language> lang);

protected:
    const char *_name;
    std::stringstream _head;
    std::stringstream _tail;
};


#endif
