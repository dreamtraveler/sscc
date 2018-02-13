#ifndef __LEX_H__
#define __LEX_H__

#include "token.h"

class Lex {
public:
    Lex();
    std::shared_ptr<Token> get(Input &input);

};

#endif

