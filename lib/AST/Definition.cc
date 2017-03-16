#include "AST/Definition.h"

Scheme::Definition::Definition(SchemeObjectPtr var, SchemeObjectPtr val) :
    Scheme::SchemeObject(Scheme::SchemeObject::DEFINE_TY),
    var_(var), val_(val)
{
}

Scheme::Definition::~Definition() {
}

Scheme::SchemeObjectPtr Scheme::Definition::getVariable() const {
    return var_;
}

Scheme::SchemeObjectPtr Scheme::Definition::getValue() const {
    return val_;
}
