#include "AST/Boolean.h"

Scheme::Boolean const* Scheme::Boolean::dummy_true =
        new Scheme::Boolean(new Token(-1, -1, 2, "#t", Scheme::TokenType::T_BOOLEAN));

Scheme::Boolean const* Scheme::Boolean::dummy_false =
        new Scheme::Boolean(new Token(-1, -1, 2, "#f", Scheme::TokenType::T_BOOLEAN));

Scheme::Boolean::Boolean(Scheme::Token* value) :
    SchemeObject(Scheme::SchemeObject::BOOLEAN_TY),
    value_(value)
{}

Scheme::Boolean::~Boolean() {
}

Scheme::Boolean const* Scheme::Boolean::get(Scheme::Token* value) {
    if (value->getText() == "#t") {
        return dummy_true;
    } else {
        return dummy_false;
    }
}

bool Scheme::Boolean::isTrue() const {
    return this == dummy_true;
}

bool Scheme::Boolean::isFalse() const {
    return not isTrue();
}

Scheme::Token* Scheme::Boolean::getValue() const {
    return value_;
}

Scheme::Boolean const* Scheme::Boolean::getTrue() {
    return dummy_true;
}

Scheme::Boolean const* Scheme::Boolean::getFalse() {
    return dummy_false;
}
