#include "AST/Character.h"

Scheme::Character::Character(char value) :
    SchemeObject(Scheme::SchemeObject::CHARACTER_TY), value_(value)
{}

Scheme::Character::~Character() {
}

char Scheme::Character::getValue() const {
    return value_;
}
