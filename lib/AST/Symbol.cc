#include "AST/Symbol.h"

Scheme::Symbol::Symbol(Token* value) :
    SchemeObject(Scheme::SchemeObject::SYMBOL_TY), value_(value)
{
}

Scheme::Symbol::~Symbol() {
}

Scheme::Symbol* Scheme::Symbol::getSymbol(Token* value) {
    static std::map<std::string, Symbol*> cache_;
    std::string const& name = value->getText();
    if (cache_.find(name) == cache_.end()) {
        cache_[name] = new Symbol(value);
    }
    return cache_[name];
}

Token* Scheme::Symbol::getValue() const {
    return value_;
}
