#include "AST/String.h"

Scheme::String::String(std::shared_ptr<Token> value) :
    SchemeObject(Scheme::SchemeObject::STRING_TY), value_(value)
{
}

Scheme::String::~String() {
}

std::shared_ptr<Scheme::Token> Scheme::String::getValue() const {
    return value_;
}
