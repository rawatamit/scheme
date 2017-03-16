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
    bool isSelfEvaluating(Scheme::SchemeObjectPtr obj) {
        return obj->isBoolean() or obj->isFixnum()
            or obj->isCharacter() or obj->isString();
    }

    bool isVariable(Scheme::SchemeObjectPtr obj) {
        return obj->isSymbol();
    }

    bool isApplication(Scheme::SchemeObjectPtr obj) {
        return obj->isPair();
    }

    Scheme::SchemeObjectPtr getOperator(Scheme::SchemeObjectPtr obj) {
        return std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCar();
    }

    Scheme::SchemeObjectPtr getOperands(Scheme::SchemeObjectPtr obj) {
        return std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCdr();
    }
} // anonymous namespace

Scheme::SchemeObjectPtr Scheme::eval(Scheme::SchemeObjectPtr obj,
                                     std::shared_ptr<Scheme::Environment> env)
{
    if (isSelfEvaluating(obj)) {
        return obj;
    } else if (isVariable(obj)) {
        auto var = std::dynamic_pointer_cast<Scheme::Symbol>(obj);
        Scheme::SchemeObjectPtr val = env->getCurrentFrame()->lookup(var->getValue()->getText());
        if (val != nullptr) {
            return val;
        } else {
            throw EvalException("variable not bound to a value");
        }
    } else if (obj->isQuote()) {
        return std::dynamic_pointer_cast<Scheme::Quote>(obj)->getTextOfQuotation();
    } else if (obj->isDefinition()) {
        auto def = std::dynamic_pointer_cast<Scheme::Definition>(obj);
        auto val = eval(def->getValue(), env);
        auto var = std::dynamic_pointer_cast<Scheme::Symbol>(def->getVariable());
        env->getCurrentFrame()->define(var->getValue()->getText(), val);
        return val;
    } else if (obj->isRedefinition()) {
        auto redef = std::dynamic_pointer_cast<Scheme::Redefinition>(obj);
        auto val = eval(redef->getValue(), env);
        auto var = std::dynamic_pointer_cast<Scheme::Symbol>(redef->getVariable());
        env->getCurrentFrame()->redefine(var->getValue()->getText(), val);
        return val;
    } else if (obj->isIf()) {
        auto ifform = std::dynamic_pointer_cast<Scheme::If>(obj);
        auto predval = eval(ifform->getPredicate(), env);

        // if the predicate returned a boolean value
        // check whether it is false, if so then the condition is false
        // otherwise the condition is true
        if (auto boolval = std::dynamic_pointer_cast<Scheme::Boolean>(predval)) {
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
        return std::dynamic_pointer_cast<Scheme::Procedure>(procedure)->getFunction()(operands);
    }
    
    // something went wrong
    throw new EvalException("error evaluating scheme form");
}

Scheme::SchemeObjectPtr Scheme::listOfValues(Scheme::SchemeObjectPtr obj,
                                             std::shared_ptr<Scheme::Environment> env)
{
    if (obj->isEmptyList()) {
        return obj;
    } else {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(obj);
        return std::make_shared<Scheme::Pair>(-1, -1, -1, -1, eval(list->getCar(), env), listOfValues(list->getCdr(), env));
    }
}
