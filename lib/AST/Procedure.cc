#include "AST/Procedure.h"
#include "AST/Pair.h"
#include "AST/Fixnum.h"
#include "AST/Boolean.h"
#include "AST/Character.h"

Scheme::Procedure::Procedure(Scheme::Procedure::FunctionType f) :
    Scheme::SchemeObject(Scheme::SchemeObject::PROC_TY),
    f_(f)
{}

Scheme::Procedure::~Procedure()
{}

Scheme::Procedure::FunctionType Scheme::Procedure::getFunction() const
{
    return f_;
}

// builtins
#define TYPE_PREDICATE(NAME, PREDICATE) Scheme::SchemeObjectPtr Scheme::NAME(Scheme::SchemeObjectPtr arguments) { \
    return std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar()->PREDICATE() ? Scheme::Boolean::getTrue() \
                                                                                      : Scheme::Boolean::getFalse(); \
}

TYPE_PREDICATE(is_null_builtin, isEmptyList);
TYPE_PREDICATE(is_boolean_builtin, isBoolean);
TYPE_PREDICATE(is_symbol_builtin, isSymbol);
TYPE_PREDICATE(is_fixnum_builtin, isFixnum);
TYPE_PREDICATE(is_char_builtin, isCharacter);
TYPE_PREDICATE(is_string_builtin, isString);
TYPE_PREDICATE(is_pair_builtin, isPair);
TYPE_PREDICATE(is_procedure_builtin, isProcedure);

#define TYPE_CONVERSION(NAME, FROM_CLASS, TO_CLASS, FUNCTION) \
Scheme::SchemeObject const* Scheme::NAME(Scheme::SchemeObject const* arguments) { \
    auto val = dynamic_cast<FROM_CLASS>(dynamic_cast<Scheme::Pair>(arguments)->getCar())->getValue(); \
    return new TO_CLASS(FUNCTION(val)); \
}

// FIXME: finish type conversion procedures, might require an AST rewrite
#if 0
TYPE_CONVERSION(char_to_fixnum_builtin, Scheme::Character, Scheme::Fixnum,
                [] (Scheme::Token* token) {
                    char buf[64];
                    snprintf(buf, sizeof buf, "%ld", token->getText());
                    return new Token(-1, -1, -1, buf, TokenType::T_FIXNUM); });
#endif

Scheme::SchemeObjectPtr Scheme::add_builtin(Scheme::SchemeObjectPtr arguments) {
    long ret = 0;

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            ret += atoi(fixnum->getValue()->getText().c_str());
        }

        arguments = list->getCdr();
    }

    char buf[128];
    snprintf(buf, sizeof buf, "%ld", ret);
    return std::make_shared<Scheme::Fixnum>(std::make_shared<Token>(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObjectPtr Scheme::sub_builtin(Scheme::SchemeObjectPtr arguments) {
    long ret = atoi(std::dynamic_pointer_cast<Scheme::Fixnum>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue()->getText().c_str());
    arguments = std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            ret -= atoi(fixnum->getValue()->getText().c_str());
        }

        arguments = list->getCdr();
    }

    char buf[128];
    snprintf(buf, sizeof buf, "%ld", ret);
    return std::make_shared<Scheme::Fixnum>(std::make_shared<Token>(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObjectPtr Scheme::mul_builtin(Scheme::SchemeObjectPtr arguments) {
    long ret = 1;

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            ret *= atoi(fixnum->getValue()->getText().c_str());
        }

        arguments = list->getCdr();
    }

    char buf[128];
    snprintf(buf, sizeof buf, "%ld", ret);
    return std::make_shared<Scheme::Fixnum>(std::make_shared<Token>(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObjectPtr Scheme::quotient_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto a = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar());
    auto b = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCadr());

    char buf[128];
    long ret = atoi(a->getValue()->getText().c_str()) / atoi(b->getValue()->getText().c_str());
    snprintf(buf, sizeof buf, "%ld", ret);
    return std::make_shared<Scheme::Fixnum>(std::make_shared<Token>(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObjectPtr Scheme::remainder_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto a = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar());
    auto b = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCadr());

    char buf[128];
    long ret = atoi(a->getValue()->getText().c_str()) % atoi(b->getValue()->getText().c_str());
    snprintf(buf, sizeof buf, "%ld", ret);
    return std::make_shared<Scheme::Fixnum>(std::make_shared<Token>(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObjectPtr Scheme::equal_fixnum_builtin(Scheme::SchemeObjectPtr arguments) {
    long val = atoi(std::dynamic_pointer_cast<Scheme::Fixnum>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue()->getText().c_str());
    arguments = std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            if (val != atoi(fixnum->getValue()->getText().c_str())) {
                return Scheme::Boolean::getFalse();
            }
        }

        arguments = list->getCdr();
    }

    return Scheme::Boolean::getTrue();
}

Scheme::SchemeObjectPtr Scheme::lt_fixnum_builtin(Scheme::SchemeObjectPtr arguments) {
    long val = atoi(std::dynamic_pointer_cast<Scheme::Fixnum>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue()->getText().c_str());
    arguments = std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            long cur = atoi(fixnum->getValue()->getText().c_str());

            if (val < cur) {
                val = cur;
            } else {
                return Scheme::Boolean::getFalse();
            }
        }

        arguments = list->getCdr();
    }

    return Scheme::Boolean::getTrue();
}

Scheme::SchemeObjectPtr Scheme::gt_fixnum_builtin(Scheme::SchemeObjectPtr arguments) {
    long val = atoi(std::dynamic_pointer_cast<Scheme::Fixnum>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue()->getText().c_str());
    arguments = std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            long cur = atoi(fixnum->getValue()->getText().c_str());

            if (val > cur) {
                val = cur;
            } else {
                return Scheme::Boolean::getFalse();
            }
        }

        arguments = list->getCdr();
    }

    return Scheme::Boolean::getTrue();
}

Scheme::SchemeObjectPtr Scheme::cons_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    return std::make_shared<Scheme::Pair>(-1, -1, -1, -1, list->getCar(), list->getCadr());
}

Scheme::SchemeObjectPtr Scheme::car_builtin(Scheme::SchemeObjectPtr arguments) {
    return std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCaar();
}

Scheme::SchemeObjectPtr Scheme::cdr_builtin(Scheme::SchemeObjectPtr arguments) {
    return std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdar();
}

Scheme::SchemeObjectPtr Scheme::list_builtin(Scheme::SchemeObjectPtr arguments) {
    return arguments;
}
