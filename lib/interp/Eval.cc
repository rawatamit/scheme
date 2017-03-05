#include <AST/Procedure.h>
#include "interp/Eval.h"
#include "interp/EvalException.h"
#include "AST/Boolean.h"
#include "AST/Symbol.h"
#include "AST/Pair.h"
#include "AST/Quote.h"
#include "AST/Definition.h"
#include "AST/Redefinition.h"
#include "AST/If.h"

namespace {
    bool isSelfEvaluating(Scheme::SchemeObject const* obj) {
        return obj->isBoolean() or obj->isFixnum()
            or obj->isCharacter() or obj->isString();
    }

    bool isVariable(Scheme::SchemeObject const* obj) {
        return obj->isSymbol();
    }

    bool isApplication(Scheme::SchemeObject const* obj) {
        return obj->isPair();
    }

    Scheme::SchemeObject const* getOperator(Scheme::SchemeObject const* obj) {
        return dynamic_cast<Scheme::Pair const*>(obj)->getCar();
    }

    Scheme::SchemeObject const* getOperands(Scheme::SchemeObject const* obj) {
        return dynamic_cast<Scheme::Pair const*>(obj)->getCdr();
    }
} // anonymous namespace

Scheme::SchemeObject const* Scheme::eval(Scheme::SchemeObject const* obj,
                                         Scheme::Environment* env)
{
    if (isSelfEvaluating(obj)) {
        return obj;
    } else if (isVariable(obj)) {
        auto var = dynamic_cast<const Scheme::Symbol*>(obj);
        Scheme::SchemeObject const* val = env->getCurrentFrame()->lookup(var->getValue()->getText());
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
        auto var = dynamic_cast<const Scheme::Symbol*>(def->getVariable());
        env->getCurrentFrame()->define(var->getValue()->getText(), val);
        return val;
    } else if (obj->isRedefinition()) {
        Scheme::Redefinition const* redef = dynamic_cast<Scheme::Redefinition const*>(obj);
        Scheme::SchemeObject const* val = eval(redef->getValue(), env);
        auto var = dynamic_cast<const Scheme::Symbol*>(redef->getVariable());
        env->getCurrentFrame()->redefine(var->getValue()->getText(), val);
        return val;
    } else if (obj->isIf()) {
        auto ifform = dynamic_cast<Scheme::If const*>(obj);
        auto predval = eval(ifform->getPredicate(), env);

        // if the predicate returned a boolean value
        // check whether it is false, if so then the condition is false
        // otherwise the condition is true
        if (auto boolval = dynamic_cast<Scheme::Boolean const*>(predval)) {
            if (boolval->isFalse()) {
                // if this if form has a non-null alternative,
                // evaluate it, else return the false value
                return ifform->getAlternative() == nullptr ? Scheme::Boolean::getFalse()
                                                           : eval(ifform->getAlternative(), env);
            } else {
                // return the consequent after evaluating
                return eval(ifform->getConsequent(), env);
            }
        } else {
            // return the consequent after evaluating
            return eval(ifform->getConsequent(), env);
        }
    } else if (isApplication(obj)) {
        auto procedure = eval(getOperator(obj), env);
        auto operands = listOfValues(getOperands(obj), env);
        return dynamic_cast<Scheme::Procedure const*>(procedure)->getFunction()(operands);
    }
    
    // something went wrong
    throw new EvalException("error evaluating scheme form");
}

Scheme::SchemeObject const* Scheme::listOfValues(Scheme::SchemeObject const* obj,
                                         Scheme::Environment* env)
{
    if (obj->isEmptyList()) {
        return obj;
    } else {
        auto list = dynamic_cast<Scheme::Pair const*>(obj);
        return new Scheme::Pair(-1, -1, -1, -1, eval(list->getCar(), env), listOfValues(list->getCdr(), env));
    }
}
