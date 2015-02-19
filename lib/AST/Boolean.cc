#include "AST/Boolean.h"

Scheme::Boolean::Boolean(Token* value, bool isTrue) :
    SchemeObject(Scheme::SchemeObject::BOOLEAN_TY),
    value_(value), isTrue_(isTrue)
{
}

Scheme::Boolean::~Boolean() {
}

Scheme::Boolean* Scheme::Boolean::get(Token* value) {
    if (value->getText() == "#t") {
        return new Boolean(value, true);
    } else {
        return new Boolean(value, false);
    }
}

bool Scheme::Boolean::isTrue() const {
    return isTrue_;
}

bool Scheme::Boolean::isFalse() const {
    return not isTrue_;
}

Token* Scheme::Boolean::getValue() const {
    return value_;
}
