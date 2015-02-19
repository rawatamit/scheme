#include "AST/String.h"

Scheme::String::String(Token* value) :
    SchemeObject(Scheme::SchemeObject::STRING_TY), value_(value)
{
}

Scheme::String::~String() {
}

Token* Scheme::String::getValue() const {
    return value_;
}
