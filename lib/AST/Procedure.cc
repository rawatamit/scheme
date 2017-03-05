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
#define TYPE_PREDICATE(NAME, PREDICATE) Scheme::SchemeObject const* Scheme::NAME(Scheme::SchemeObject const* arguments) { \
    return dynamic_cast<Scheme::Pair const*>(arguments)->getCar()->PREDICATE() ? Scheme::Boolean::getTrue() \
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
    auto val = dynamic_cast<FROM_CLASS const*>(dynamic_cast<Scheme::Pair const*>(arguments)->getCar())->getValue(); \
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

Scheme::SchemeObject const* Scheme::add_builtin(Scheme::SchemeObject const *arguments) {
    long ret = 0;

    while (not arguments->isEmptyList()) {
        auto list = dynamic_cast<Scheme::Pair const*>(arguments);

        if (auto fixnum = dynamic_cast<Scheme::Fixnum const*>(list->getCar())) {
            ret += atoi(fixnum->getValue()->getText().c_str());
        }

        arguments = list->getCdr();
    }

    char buf[128];
    snprintf(buf, sizeof buf, "%ld", ret);
    return new Scheme::Fixnum(new Token(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObject const* Scheme::sub_builtin(Scheme::SchemeObject const *arguments) {
    long ret = atoi(dynamic_cast<Scheme::Fixnum const*>(
                        dynamic_cast<Scheme::Pair const*>(arguments)->getCar())->getValue()->getText().c_str());
    arguments = dynamic_cast<Scheme::Pair const*>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = dynamic_cast<Scheme::Pair const*>(arguments);

        if (auto fixnum = dynamic_cast<Scheme::Fixnum const*>(list->getCar())) {
            ret -= atoi(fixnum->getValue()->getText().c_str());
        }

        arguments = list->getCdr();
    }

    char buf[128];
    snprintf(buf, sizeof buf, "%ld", ret);
    return new Scheme::Fixnum(new Token(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObject const* Scheme::mul_builtin(Scheme::SchemeObject const *arguments) {
    long ret = 1;

    while (not arguments->isEmptyList()) {
        auto list = dynamic_cast<Scheme::Pair const*>(arguments);

        if (auto fixnum = dynamic_cast<Scheme::Fixnum const*>(list->getCar())) {
            ret *= atoi(fixnum->getValue()->getText().c_str());
        }

        arguments = list->getCdr();
    }

    char buf[128];
    snprintf(buf, sizeof buf, "%ld", ret);
    return new Scheme::Fixnum(new Token(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObject const* Scheme::quotient_builtin(const SchemeObject *arguments) {
    auto list = dynamic_cast<Scheme::Pair const*>(arguments);
    auto a = dynamic_cast<Scheme::Fixnum const*>(list->getCar());
    auto b = dynamic_cast<Scheme::Fixnum const*>(list->getCadr());

    char buf[128];
    long ret = atoi(a->getValue()->getText().c_str()) / atoi(b->getValue()->getText().c_str());
    snprintf(buf, sizeof buf, "%ld", ret);
    return new Scheme::Fixnum(new Token(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObject const* Scheme::remainder_builtin(const SchemeObject *arguments) {
    auto list = dynamic_cast<Scheme::Pair const*>(arguments);
    auto a = dynamic_cast<Scheme::Fixnum const*>(list->getCar());
    auto b = dynamic_cast<Scheme::Fixnum const*>(list->getCadr());

    char buf[128];
    long ret = atoi(a->getValue()->getText().c_str()) % atoi(b->getValue()->getText().c_str());
    snprintf(buf, sizeof buf, "%ld", ret);
    return new Scheme::Fixnum(new Token(-1, -1, strlen(buf), buf, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObject const* Scheme::equal_fixnum_builtin(const SchemeObject *arguments) {
    long val = atoi(dynamic_cast<Scheme::Fixnum const*>(
                            dynamic_cast<Scheme::Pair const*>(arguments)->getCar())->getValue()->getText().c_str());
    arguments = dynamic_cast<Scheme::Pair const*>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = dynamic_cast<Scheme::Pair const*>(arguments);

        if (auto fixnum = dynamic_cast<Scheme::Fixnum const*>(list->getCar())) {
            if (val != atoi(fixnum->getValue()->getText().c_str())) {
                return Scheme::Boolean::getFalse();
            }
        }

        arguments = list->getCdr();
    }

    return Scheme::Boolean::getTrue();
}

Scheme::SchemeObject const* Scheme::lt_fixnum_builtin(const SchemeObject *arguments) {
    long val = atoi(dynamic_cast<Scheme::Fixnum const*>(
                            dynamic_cast<Scheme::Pair const*>(arguments)->getCar())->getValue()->getText().c_str());
    arguments = dynamic_cast<Scheme::Pair const*>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = dynamic_cast<Scheme::Pair const*>(arguments);

        if (auto fixnum = dynamic_cast<Scheme::Fixnum const*>(list->getCar())) {
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

Scheme::SchemeObject const* Scheme::gt_fixnum_builtin(const SchemeObject *arguments) {
    long val = atoi(dynamic_cast<Scheme::Fixnum const*>(
                            dynamic_cast<Scheme::Pair const*>(arguments)->getCar())->getValue()->getText().c_str());
    arguments = dynamic_cast<Scheme::Pair const*>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = dynamic_cast<Scheme::Pair const*>(arguments);

        if (auto fixnum = dynamic_cast<Scheme::Fixnum const*>(list->getCar())) {
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

Scheme::SchemeObject const* Scheme::cons_builtin(const SchemeObject *arguments) {
    auto list = dynamic_cast<Scheme::Pair const*>(arguments);
    return new Scheme::Pair(-1, -1, -1, -1, list->getCar(), list->getCadr());
}

Scheme::SchemeObject const* Scheme::car_builtin(const SchemeObject *arguments) {
    return dynamic_cast<Scheme::Pair const*>(arguments)->getCaar();
}

Scheme::SchemeObject const* Scheme::cdr_builtin(const SchemeObject *arguments) {
    return dynamic_cast<Scheme::Pair const*>(arguments)->getCdar();
}

Scheme::SchemeObject const* Scheme::list_builtin(const SchemeObject *arguments) {
    return arguments;
}
