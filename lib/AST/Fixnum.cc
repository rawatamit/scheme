#include "AST/Fixnum.h"

Scheme::Fixnum::Fixnum(long value) :
    Scheme::SchemeObject(Scheme::SchemeObject::FIXNUM_TY), value_(value)
{}

Scheme::Fixnum::~Fixnum() {
}

long Scheme::Fixnum::getValue() const {
    return value_;
}
