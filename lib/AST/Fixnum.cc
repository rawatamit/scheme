#include "AST/Fixnum.h"

Scheme::Fixnum::Fixnum(Token* value) :
    Scheme::SchemeObject(Scheme::SchemeObject::FIXNUM_TY), value_(value)
{
}

Scheme::Fixnum::~Fixnum() {
}

Token* Scheme::Fixnum::getValue() const {
    return value_;
}
