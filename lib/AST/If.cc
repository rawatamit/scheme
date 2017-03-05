#include "AST/If.h"

Scheme::If::If(SchemeObject const* predicate, SchemeObject const* consequent, SchemeObject const* alternative) :
    Scheme::SchemeObject(Scheme::SchemeObject::IF_TY),
    predicate_(predicate), consequent_(consequent), alternative_(alternative)
{}

Scheme::If::~If()
{}

Scheme::SchemeObject const* Scheme::If::getPredicate() const {
    return predicate_;
}

Scheme::SchemeObject const* Scheme::If::getConsequent() const {
    return consequent_;
}

Scheme::SchemeObject const* Scheme::If::getAlternative() const {
    return alternative_;
}
