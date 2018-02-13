#ifndef __PARSE_H__
#define __PARSE_H__

#include "input.h"
#include "token.h"
#include "lex.h"

#include "symtab.h"

class Lex;
extern const char *the_request_name;
extern const char *the_response_name;

enum {
    PARSE_PHASE_HEAD,
    PARSE_PHASE_BODY,
    PARSE_PHASE_TAIL,
};

class Parser {
public:
    Parser(Input &input, SymbolTable &symbols);
    int phase() const {
        return _phase;
    }
    bool parse();

    std::shared_ptr<Token> cur();
    void eat();
    void eat(Location &loc);
    std::shared_ptr<Token> look();

    Input &input() {
        return _input;
    }

    SymbolTable &symbols() {
        return _symbols;
    }

    bool skip_newline() const {
        return _skip_newline;
    }

    bool skip_newline(bool value) {
        bool old = _skip_newline;
        _skip_newline = value;
        return old;
    }

private:
    void parse_head(std::shared_ptr<Token> token);
    void parse_body(std::shared_ptr<Token> token);
    void parse_tail(std::shared_ptr<Token> token);
    void parse_seg(std::shared_ptr<Token> token);
    void prase_option();
    void parse_raw(std::stringstream *stream);
    void parse_enum();
    void parse_extern();
private:
    int _phase;
	std::shared_ptr<Lex> _lex;
    std::shared_ptr<Token> _queue[2];
    unsigned _index;
    Input &_input;
    SymbolTable &_symbols;
    bool _skip_newline;
};

#endif

