#include "AST/Redefinition.h"

Scheme::Redefinition::Redefinition(SchemeObject const* var, SchemeObject const* val) :
    Scheme::SchemeObject(Scheme::SchemeObject::REDEFINE_TY),
    var_(var), val_(val)
{
}

Scheme::Redefinition::~Redefinition() {
}

Scheme::SchemeObject const* Scheme::Redefinition::getVariable() const {
    return var_;
}

Scheme::SchemeObject const* Scheme::Redefinition::getValue() const {
    return val_;
}
