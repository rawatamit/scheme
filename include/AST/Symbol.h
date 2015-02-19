#ifndef SYMBOL_H
#define SYMBOL_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Symbol : public SchemeObject {
private:
    std::set<Symbol*> symbols_;
public:
    Symbol(Token* value);
    virtual ~Symbol();
    Token* getValue() const;
};
} // namespace Scheme

#endif
