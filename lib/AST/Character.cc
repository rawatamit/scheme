#include "AST/Character.h"

Scheme::Character::Character(Token* value) :
    SchemeObject(Scheme::SchemeObject::CHARACTER_TY), value_(value)
{
}

Scheme::Character::~Character() {
}

Token* Scheme::Character::getValue() const {
    return value_;
}
