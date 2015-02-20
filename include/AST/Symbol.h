#ifndef SYMBOL_H
#define SYMBOL_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"
#include <map>

namespace Scheme {
class Symbol : public SchemeObject {
public:    
    Token* value_;
    Symbol(Token* value);
public:
    static Symbol* getSymbol(Token* value);
    virtual ~Symbol();
    Token* getValue() const;
};
} // namespace Scheme

#endif
