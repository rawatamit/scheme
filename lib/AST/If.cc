#include "AST/If.h"

Scheme::If::If(SchemeObjectPtr predicate, SchemeObjectPtr consequent, SchemeObjectPtr alternative) :
    Scheme::SchemeObject(Scheme::SchemeObject::IF_TY),
    predicate_(predicate), consequent_(consequent), alternative_(alternative)
{}

Scheme::If::~If()
{}

Scheme::SchemeObjectPtr Scheme::If::getPredicate() const {
    return predicate_;
}

Scheme::SchemeObjectPtr Scheme::If::getConsequent() const {
    return consequent_;
}

Scheme::SchemeObjectPtr Scheme::If::getAlternative() const {
    return alternative_;
}
