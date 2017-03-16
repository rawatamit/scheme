#include "AST/Symbol.h"

Scheme::Symbol::Symbol(std::shared_ptr<Scheme::Token> value) :
    SchemeObject(Scheme::SchemeObject::SYMBOL_TY), value_(value)
{
}

Scheme::Symbol::~Symbol() {
}

Scheme::SchemeObjectPtr Scheme::Symbol::getSymbol(std::shared_ptr<Scheme::Token> value) {
    static std::map<std::string, SchemeObjectPtr> cache_;
    std::string const& name = value->getText();
    if (cache_.find(name) == cache_.end()) {
        cache_[name] = std::make_shared<Symbol>(value);
    }
    return cache_[name];
}

std::shared_ptr<Scheme::Token> Scheme::Symbol::getValue() const {
    return value_;
}
