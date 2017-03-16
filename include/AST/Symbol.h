#ifndef SYMBOL_H
#define SYMBOL_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"
#include <map>

namespace Scheme {
class Symbol : public SchemeObject {
public:    
    std::shared_ptr<Token> value_;
    Symbol(std::shared_ptr<Token> value);
public:
    static SchemeObjectPtr getSymbol(std::shared_ptr<Token> value);
    virtual ~Symbol();
    std::shared_ptr<Token> getValue() const;
};
} // namespace Scheme

#endif
