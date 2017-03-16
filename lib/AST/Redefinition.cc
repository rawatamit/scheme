#include "AST/Redefinition.h"

Scheme::Redefinition::Redefinition(SchemeObjectPtr var, SchemeObjectPtr val) :
    Scheme::SchemeObject(Scheme::SchemeObject::REDEFINE_TY),
    var_(var), val_(val)
{
}

Scheme::Redefinition::~Redefinition() {
}

Scheme::SchemeObjectPtr Scheme::Redefinition::getVariable() const {
    return var_;
}

Scheme::SchemeObjectPtr Scheme::Redefinition::getValue() const {
    return val_;
}
