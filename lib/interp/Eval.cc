#include "AST/Procedure.h"
#include "AST/CompoundProcedure.h"
#include "AST/Boolean.h"
#include "AST/Symbol.h"
#include "AST/Pair.h"
#include "AST/SchemeEnvironment.h"
#include "interp/Eval.h"
#include "interp/EvalException.h"

namespace {
    bool isSelfEvaluating(Scheme::SchemeObjectPtr obj) {
        return obj->isBoolean() or obj->isFixnum()
            or obj->isCharacter() or obj->isString();
    }

    bool isVariable(Scheme::SchemeObjectPtr obj) {
        return obj->isSymbol();
    }

    bool isTagged(Scheme::SchemeObjectPtr obj, const std::string& tag) {
        if (auto list = std::dynamic_pointer_cast<Scheme::Pair>(obj)) {
            if (auto car = std::dynamic_pointer_cast<Scheme::Symbol>(list->getCar())) {
                return car->getValue()->getText() == tag;
            } else {
                return false;
            }
        }

        return false;
    }

    bool isQuote(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "quote");
    }

    bool isIf(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "if");
    }

    bool isDefinition(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "define");
    }

    bool isRedefinition(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "set!");
    }

    bool isLambda(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "lambda");
    }

    bool isBegin(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "begin");
    }

    bool isCond(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "cond");
    }

    bool isLet(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "let");
    }

    bool isAnd(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "and");
    }

    bool isOr(Scheme::SchemeObjectPtr obj) {
        return isTagged(obj, "or");
    }

    bool isApplication(Scheme::SchemeObjectPtr obj) {
        return obj->isPair();
    }

    bool isLastExpression(Scheme::SchemeObjectPtr obj) {
        return obj->isEmptyList() or std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCdr()->isEmptyList();
    }

    Scheme::SchemeObjectPtr textOfQuotation(Scheme::SchemeObjectPtr obj) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(obj);

        if (list->getCdr() == nullptr) {
            throw Scheme::EvalException("quote form not proper");
        } else if (list->getCdr()->isEmptyList()) {
            return list->getCdr();
        } else {
            auto pcdr = std::dynamic_pointer_cast<Scheme::Pair>(list->getCdr());
            return pcdr->getCar();
        }
    }

    Scheme::SchemeObjectPtr getOperator(Scheme::SchemeObjectPtr obj) {
        return std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCar();
    }

    Scheme::SchemeObjectPtr getOperands(Scheme::SchemeObjectPtr obj) {
        return std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCdr();
    }

    Scheme::SchemeObjectPtr sequence_to_exp(Scheme::SchemeObjectPtr seq) {
        if (seq->isEmptyList()) {
            return seq;
        } else if (isLastExpression(seq)) {
            return std::dynamic_pointer_cast<Scheme::Pair>(seq)->getCar();
        } else {
            return Scheme::Pair::cons(Scheme::SchemeObject::begin_symbol,
                                      Scheme::Pair::cons(seq, Scheme::Pair::getEmptyList()));
        }
    }

    Scheme::SchemeObjectPtr cond_predicate(Scheme::SchemeObjectPtr obj) {
        return std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCar();
    }

    Scheme::SchemeObjectPtr cond_actions(Scheme::SchemeObjectPtr obj) {
        return std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCdr();
    }

    bool is_else_clause(Scheme::SchemeObjectPtr obj) {
        if (auto sym = std::dynamic_pointer_cast<Scheme::Symbol>(cond_predicate(obj))) {
            return sym->getValue()->getText() == "else";
        }

        return false;
    }

    Scheme::SchemeObjectPtr expand_clauses(Scheme::SchemeObjectPtr clauses) {
        if (clauses->isEmptyList()) {
            return clauses;
        } else {
            auto list = std::dynamic_pointer_cast<Scheme::Pair>(clauses);
            auto first = list->getCar();
            auto rest = list->getCdr();

            if (is_else_clause(first)) {
                if (rest->isEmptyList()) {
                    return sequence_to_exp(cond_actions(first));
                } else {
                    throw Scheme::EvalException("else clause isn't last cond->if");
                }
            } else {
                return Scheme::Pair::cons(Scheme::SchemeObject::if_symbol,
                            Scheme::Pair::cons(cond_predicate(first),
                                Scheme::Pair::cons(sequence_to_exp(cond_actions(first)),
                                    Scheme::Pair::cons(expand_clauses(rest),
                                        Scheme::Pair::getEmptyList()))));
            }
        }
    }

    Scheme::SchemeObjectPtr cond_to_if(Scheme::SchemeObjectPtr obj) {
        return expand_clauses(std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCdr());
    }

    Scheme::SchemeObjectPtr binding_parameter(Scheme::SchemeObjectPtr obj) {
        return std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCar();
    }

    Scheme::SchemeObjectPtr bindings_parameters(Scheme::SchemeObjectPtr obj) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(obj);
        return list->isEmptyList() ? list
                                   : Scheme::Pair::cons(binding_parameter(list->getCar()),
                                                        bindings_parameters(list->getCdr()));
    }

    Scheme::SchemeObjectPtr binding_argument(Scheme::SchemeObjectPtr obj) {
        return std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCadr();
    }

    Scheme::SchemeObjectPtr bindings_arguments(Scheme::SchemeObjectPtr obj) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(obj);
        return list->isEmptyList() ? list
                                   : Scheme::Pair::cons(binding_argument(list->getCar()),
                                                        bindings_arguments(list->getCdr()));
    }

    Scheme::SchemeObjectPtr let_to_application(Scheme::SchemeObjectPtr obj) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(obj);
        auto parameters = bindings_parameters(list->getCadr());
        auto arguments = bindings_arguments(list->getCadr());
        return Scheme::Pair::cons(
                Scheme::Pair::cons(Scheme::SchemeObject::lambda_symbol,
                    Scheme::Pair::cons(parameters, list->getCddr())),
                arguments);
    }

    Scheme::SchemeObjectPtr apply_operator(Scheme::SchemeObjectPtr args) {
        return std::dynamic_pointer_cast<Scheme::Pair>(args)->getCar();
    }

    Scheme::SchemeObjectPtr get_apply_operands(Scheme::SchemeObjectPtr args) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(args);

        if (list->getCdr()->isEmptyList()) {
            return list->getCar();
        } else {
            return std::make_shared<Scheme::Pair>(list->getCar(), get_apply_operands(list->getCdr()));
        }
    }

    Scheme::SchemeObjectPtr apply_operands(Scheme::SchemeObjectPtr args) {
        return get_apply_operands(std::dynamic_pointer_cast<Scheme::Pair>(args)->getCdr());
    }

    Scheme::SchemeObjectPtr eval_expression(Scheme::SchemeObjectPtr args) {
        return std::dynamic_pointer_cast<Scheme::Pair>(args)->getCar();
    }

    Scheme::EnvironmentPtr eval_environment(Scheme::SchemeObjectPtr args) {
        return std::dynamic_pointer_cast<Scheme::SchemeEnvironment>(
                   std::dynamic_pointer_cast<Scheme::Pair>(args)->getCadr())->getEnvironment();
    }
} // anonymous namespace

//#include <iostream>
Scheme::SchemeObjectPtr Scheme::eval(Scheme::SchemeObjectPtr obj,
                                     Scheme::EnvironmentPtr env)
{
tailcall:
    if (isSelfEvaluating(obj)) {
        return obj;
    } else if (isVariable(obj)) {
        auto var = std::dynamic_pointer_cast<Scheme::Symbol>(obj);
        Scheme::SchemeObjectPtr val = env->getCurrentFrame()->lookup(var->getValue()->getText());
        if (val != nullptr) {
            return val;
        } else {
            std::string msg{"variable not bound to a value: "};
            msg.append(var->getValue()->getText());
            throw EvalException(msg);
        }
    } else if (isQuote(obj)) {
        return textOfQuotation(obj);
    } else if (isDefinition(obj)) {
        auto pair = std::dynamic_pointer_cast<Scheme::Pair>(obj);

        if (auto list = std::dynamic_pointer_cast<Scheme::Pair>(pair->getCdr())) {
            if (list->getCar()->isSymbol()) {
                auto var = std::dynamic_pointer_cast<Scheme::Symbol>(list->getCar())->getValue()->getText();
                auto val = eval(list->getCadr(), env);
                env->getCurrentFrame()->define(var, val);
                return val;
            } else {
                auto var = std::dynamic_pointer_cast<Scheme::Symbol>(list->getCaar())->getValue()->getText();
                auto val = eval(Scheme::Pair::cons(SchemeObject::lambda_symbol,
                                                   Scheme::Pair::cons(list->getCdar(), list->getCdr())), env);
                env->getCurrentFrame()->define(var, val);
                return val;
            }
        } else {
            throw Scheme::EvalException("define form not proper");
        }
    } else if (isRedefinition(obj)) {
        auto pair = std::dynamic_pointer_cast<Scheme::Pair>(obj);

        if (auto list = std::dynamic_pointer_cast<Scheme::Pair>(pair->getCdr())) {
            auto var = std::dynamic_pointer_cast<Scheme::Symbol>(list->getCar());
            auto val = list->getCadr();

            if (var == nullptr or val == nullptr) {
                throw Scheme::EvalException("set! form not proper");
            } else {
                val = eval(val, env);
                env->getCurrentFrame()->redefine(var->getValue()->getText(), val);
                return val;
            }
        } else {
            throw Scheme::EvalException("set! form not proper");
        }
    } else if (isIf(obj)) {
        auto pair = std::dynamic_pointer_cast<Scheme::Pair>(obj);
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(pair->getCdr());
        Scheme::SchemeObjectPtr predicate = list->getCar();
        Scheme::SchemeObjectPtr consequent = list->getCadr();
        Scheme::SchemeObjectPtr alternative = list->getCaddr();

        if (predicate == nullptr or consequent == nullptr) {
            throw Scheme::EvalException("if form not proper");
        }

        auto predval = eval(predicate, env);

        // if the predicate returned a boolean value
        // check whether it is false, if so then the condition is false
        // otherwise the condition is true
        if (auto boolval = std::dynamic_pointer_cast<Scheme::Boolean>(predval)) {
            if (boolval->isFalse()) {
                // if this if form has a non-null alternative,
                // evaluate it, else return the false value
                obj = alternative == nullptr ? Scheme::Boolean::getFalse()
                                             : alternative;
                goto tailcall;
            } else {
                // return the consequent after evaluating
                obj = consequent;
                goto tailcall;
            }
        } else {
            // return the consequent after evaluating
            obj = consequent;
            goto tailcall;
        }
    } else if (isBegin(obj)) {
        auto actions = std::dynamic_pointer_cast<Scheme::Pair>(
                            std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCdr());

        while (not isLastExpression(actions)) {
            // evaluate current expression in the new environment
            eval(actions->getCar(), env);

            // next expression in the procedure body
            actions = std::dynamic_pointer_cast<Scheme::Pair>(actions->getCdr());
        }

        obj = actions->isEmptyList() ? actions : actions->getCar();
        goto tailcall;
    } else if (isCond(obj)) {
        obj = cond_to_if(obj);
        goto tailcall;
    } else if (isLet(obj)) {
        obj = let_to_application(obj);
        goto tailcall;
    } else if (isAnd(obj)) {
        auto tests = std::dynamic_pointer_cast<Scheme::Pair>(std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCdr());

        if (tests->isEmptyList()) {
            return Scheme::Boolean::getTrue();
        } else {
            while (not isLastExpression(tests)) {
                // evaluate current test
                auto res = eval(tests->getCar(), env);

                if (res->isBoolean() and std::dynamic_pointer_cast<Scheme::Boolean>(res)->isFalse()) {
                    return res;
                }

                // next test in the and body
                tests = std::dynamic_pointer_cast<Scheme::Pair>(tests->getCdr());
            }

            obj = tests->isEmptyList() ? tests : tests->getCar();
            goto tailcall;
        }
    } else if (isOr(obj)) {
        auto tests = std::dynamic_pointer_cast<Scheme::Pair>(std::dynamic_pointer_cast<Scheme::Pair>(obj)->getCdr());

        if (tests->isEmptyList()) {
            return Scheme::Boolean::getFalse();
        } else {
            while (not isLastExpression(tests)) {
                // evaluate current test
                auto res = eval(tests->getCar(), env);

                if (res->isBoolean() and std::dynamic_pointer_cast<Scheme::Boolean>(res)->isFalse()) {
                    // next test in the or body
                    tests = std::dynamic_pointer_cast<Scheme::Pair>(tests->getCdr());
                } else {
                    return res;
                }
            }

            obj = tests->isEmptyList() ? tests : tests->getCar();
            goto tailcall;
        }
    } else if (isLambda(obj)) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(obj);
        return std::make_shared<Scheme::CompoundProcedure>(list->getCadr(), list->getCddr(), env);
    } else if (isApplication(obj)) {
        auto procedure = eval(getOperator(obj), env);
        auto arguments = listOfValues(getOperands(obj), env);

        // eval
        if (procedure->isPrimitiveProcedure() and
            std::dynamic_pointer_cast<Scheme::Procedure>(procedure)->getFunction() == eval_builtin) {
            obj = eval_expression(arguments);
            env = eval_environment(arguments);
            goto tailcall;
        }

        // apply
        if (procedure->isPrimitiveProcedure() and
            std::dynamic_pointer_cast<Scheme::Procedure>(procedure)->getFunction() == apply_builtin) {
            procedure = apply_operator(arguments);
            arguments = apply_operands(arguments);
        }

        if (procedure->isPrimitiveProcedure()) {
            return std::dynamic_pointer_cast<Scheme::Procedure>(procedure)->getFunction()(arguments);
        } else if (procedure->isCompoundProcedure()) {
            auto cproc = std::dynamic_pointer_cast<Scheme::CompoundProcedure>(procedure);
            auto nenv = std::make_shared<Scheme::Environment>(cproc->getEnvironment()); // get the environment in which the procedure was defined

            // new frame to evaluate this function
            nenv->beginFrame();

            // set the value for params
            auto params = std::dynamic_pointer_cast<Scheme::Pair>(cproc->getParameters());
            auto values = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
            while (not params->isEmptyList()) {
                // get the param and corresponding value
                auto var = std::dynamic_pointer_cast<Scheme::Symbol>(params->getCar())->getValue()->getText();
                auto val = values->getCar();

                // add this binding to the current frame
                nenv->getCurrentFrame()->define(var, val);

                // move to the next parameter
                params = std::dynamic_pointer_cast<Scheme::Pair>(params->getCdr());
                values = std::dynamic_pointer_cast<Scheme::Pair>(values->getCdr());
            }

            // evaluate the body of this procedure as a begin form
            obj = Scheme::Pair::cons(Scheme::SchemeObject::begin_symbol, cproc->getBody());
            env = nenv;
            goto tailcall;
        }
    }

    // something went wrong
    throw EvalException("error evaluating scheme form");
}

Scheme::SchemeObjectPtr Scheme::listOfValues(Scheme::SchemeObjectPtr obj,
                                             Scheme::EnvironmentPtr env)
{
    if (obj->isEmptyList()) {
        return obj;
    } else {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(obj);
        return Scheme::Pair::cons(eval(list->getCar(), env), listOfValues(list->getCdr(), env));
    }
}
