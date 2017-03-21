#include "AST/CompoundProcedure.h"

Scheme::CompoundProcedure::CompoundProcedure(Scheme::SchemeObjectPtr params, Scheme::SchemeObjectPtr body,
                                             std::shared_ptr<Scheme::Environment> env) :
    Scheme::SchemeObject(Scheme::SchemeObject::COMP_PROC_TY), params_(params), body_(body), env_(env)
{}

Scheme::CompoundProcedure::~CompoundProcedure()
{}

Scheme::SchemeObjectPtr Scheme::CompoundProcedure::getParameters() const {
    return params_;
}

Scheme::SchemeObjectPtr Scheme::CompoundProcedure::getBody() const {
    return body_;
}

std::shared_ptr<Scheme::Environment> Scheme::CompoundProcedure::getEnvironment() const {
    return env_;
}
