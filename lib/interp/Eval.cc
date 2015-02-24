#include "interp/Eval.h"
#include "interp/Environment.h"
#include "interp/EvalException.h"
#include "AST/Quote.h"
#include "AST/Definition.h"
#include "AST/Redefinition.h"
#include <stdexcept>

namespace {
inline bool isSelfEvaluating(Scheme::SchemeObject const* obj) {
    return obj->isBoolean() or obj->isFixnum()
        or obj->isCharacter() or obj->isString();
}

inline bool isVariable(Scheme::SchemeObject const* obj) {
    return obj->isSymbol();
}
} // anonymous namespace

Scheme::SchemeObject const* Scheme::eval(Scheme::SchemeObject const* obj,
                                         Scheme::Environment* env)
{
    if (isSelfEvaluating(obj)) {
        return obj;
    } else if (isVariable(obj)) {
        Scheme::SchemeObject const* val = env->getCurrentFrame()->lookup(obj);
        if (val != nullptr) {
            return val;
        } else {
            throw new EvalException("variable not bound to a value");
        }
    } else if (obj->isQuote()) {
        return dynamic_cast<Scheme::Quote const*>(obj)->getTextOfQuotation();
    } else if (obj->isDefinition()) {
        Scheme::Definition const* def = dynamic_cast<Scheme::Definition const*>(obj);
        Scheme::SchemeObject const* val = eval(def->getValue(), env);
        env->getCurrentFrame()->define(def->getVariable(), val);
        return val;
    } else if (obj->isRedefinition()) {
        Scheme::Redefinition const* redef = dynamic_cast<Scheme::Redefinition const*>(obj);
        Scheme::SchemeObject const* val = eval(redef->getValue(), env);
        env->getCurrentFrame()->redefine(redef->getVariable(), val);
        return val;
    }
    
    // something went wrong
    throw new EvalException("error evaluating scheme form");
}
