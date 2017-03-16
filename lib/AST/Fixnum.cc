#include "AST/Fixnum.h"

Scheme::Fixnum::Fixnum(std::shared_ptr<Token> value) :
    Scheme::SchemeObject(Scheme::SchemeObject::FIXNUM_TY), value_(value)
{
}

Scheme::Fixnum::~Fixnum() {
}

std::shared_ptr<Scheme::Token> Scheme::Fixnum::getValue() const {
    return value_;
}
