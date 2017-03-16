#include "AST/Character.h"

Scheme::Character::Character(std::shared_ptr<Token> value) :
    SchemeObject(Scheme::SchemeObject::CHARACTER_TY), value_(value)
{
}

Scheme::Character::~Character() {
}

std::shared_ptr<Scheme::Token> Scheme::Character::getValue() const {
    return value_;
}
