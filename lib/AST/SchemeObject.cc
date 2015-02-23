#include "AST/SchemeObject.h"

Scheme::SchemeObject::SchemeObject(Scheme::SchemeObject::ObjectTy type) :
    type_(type)
{
}

Scheme::SchemeObject::~SchemeObject() {
}

Scheme::SchemeObject::ObjectTy Scheme::SchemeObject::getType() const {
    return type_;
}

bool Scheme::SchemeObject::isFixnum() const {
    return getType() == FIXNUM_TY;
}

bool Scheme::SchemeObject::isBoolean() const {
    return getType() == BOOLEAN_TY;
}

bool Scheme::SchemeObject::isCharacter() const {
    return getType() == CHARACTER_TY;
}

bool Scheme::SchemeObject::isString() const {
    return getType() == STRING_TY;
}

bool Scheme::SchemeObject::isEmptyList() const {
    return getType() == EMPTYLIST_TY;
}

bool Scheme::SchemeObject::isPair() const {
    return getType() == PAIR_TY;
}

bool Scheme::SchemeObject::isSymbol() const {
    return getType() == SYMBOL_TY;
}

bool Scheme::SchemeObject::isQuote() const {
    return getType() == QUOTE_TY;
}
