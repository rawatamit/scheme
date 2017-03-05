#include "AST/Definition.h"

Scheme::Definition::Definition(SchemeObject const* var, SchemeObject const* val) :
    Scheme::SchemeObject(Scheme::SchemeObject::DEFINE_TY),
    var_(var), val_(val)
{
}

Scheme::Definition::~Definition() {
}

Scheme::SchemeObject const* Scheme::Definition::getVariable() const {
    return var_;
}

Scheme::SchemeObject const* Scheme::Definition::getValue() const {
    return val_;
}
