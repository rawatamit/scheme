#include "AST/Boolean.h"

Scheme::SchemeObjectPtr Scheme::Boolean::dummy_true =
        std::shared_ptr<Scheme::Boolean>(new Scheme::Boolean(
                std::make_shared<Token>(-1, -1, 2, "#t", Scheme::TokenType::T_BOOLEAN)));

Scheme::SchemeObjectPtr Scheme::Boolean::dummy_false =
        std::shared_ptr<Scheme::Boolean>(new Scheme::Boolean(
                std::make_shared<Token>(-1, -1, 2, "#f", Scheme::TokenType::T_BOOLEAN)));

Scheme::Boolean::Boolean(std::shared_ptr<Scheme::Token> value) :
    SchemeObject(Scheme::SchemeObject::BOOLEAN_TY),
    value_(value)
{}

Scheme::Boolean::~Boolean() {
}

Scheme::SchemeObjectPtr Scheme::Boolean::get(std::shared_ptr<Scheme::Token> value) {
    if (value->getText() == "#t") {
        return dummy_true;
    } else {
        return dummy_false;
    }
}

bool Scheme::Boolean::isTrue() const {
    return this == dummy_true.get();
}

bool Scheme::Boolean::isFalse() const {
    return not isTrue();
}

std::shared_ptr<Scheme::Token> Scheme::Boolean::getValue() const {
    return value_;
}

Scheme::SchemeObjectPtr Scheme::Boolean::getTrue() {
    return dummy_true;
}

Scheme::SchemeObjectPtr Scheme::Boolean::getFalse() {
    return dummy_false;
}
