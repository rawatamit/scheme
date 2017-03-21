#include "AST/SchemeObject.h"
#include "AST/Symbol.h"

Scheme::SchemeObjectPtr Scheme::SchemeObject::lambda_symbol = std::make_shared<Scheme::Symbol>(
        std::make_shared<Scheme::Token>(-1, -1, 6, "lambda", Scheme::T_SYMBOL));

Scheme::SchemeObjectPtr Scheme::SchemeObject::ok_symbol = std::make_shared<Scheme::Symbol>(
                                            std::make_shared<Scheme::Token>(-1, -1, 2, "ok", Scheme::T_SYMBOL));

Scheme::SchemeObjectPtr Scheme::SchemeObject::begin_symbol = std::make_shared<Scheme::Symbol>(
        std::make_shared<Scheme::Token>(-1, -1, 5, "begin", Scheme::T_SYMBOL));

Scheme::SchemeObjectPtr Scheme::SchemeObject::else_symbol = std::make_shared<Scheme::Symbol>(
        std::make_shared<Scheme::Token>(-1, -1, 4, "else", Scheme::T_SYMBOL));

Scheme::SchemeObject::SchemeObject(Scheme::SchemeObject::ObjectTy type) :
    type_(type)
{}

Scheme::SchemeObject::~SchemeObject() {
}

Scheme::SchemeObject::ObjectTy Scheme::SchemeObject::getType() const {
    return type_;
}

bool Scheme::SchemeObject::isFixnum() const {
    return getType() == FIXNUM_TY;
}

bool Scheme::SchemeObject::isBoolean() const {
    return getType() == BOOLEAN_TY;
}

bool Scheme::SchemeObject::isCharacter() const {
    return getType() == CHARACTER_TY;
}

bool Scheme::SchemeObject::isString() const {
    return getType() == STRING_TY;
}

bool Scheme::SchemeObject::isEmptyList() const {
    return getType() == EMPTYLIST_TY;
}

bool Scheme::SchemeObject::isPair() const {
    return getType() == PAIR_TY;
}

bool Scheme::SchemeObject::isSymbol() const {
    return getType() == SYMBOL_TY;
}

bool Scheme::SchemeObject::isQuote() const {
    return getType() == QUOTE_TY;
}

bool Scheme::SchemeObject::isDefinition() const {
    return getType() == DEFINE_TY;
}

bool Scheme::SchemeObject::isRedefinition() const {
    return getType() == REDEFINE_TY;
}

bool Scheme::SchemeObject::isIf() const {
    return getType() == IF_TY;
}

bool Scheme::SchemeObject::isProcedure() const {
    return isPrimitiveProcedure() or isCompoundProcedure();
}

bool Scheme::SchemeObject::isPrimitiveProcedure() const {
    return getType() == PROC_TY;
}

bool Scheme::SchemeObject::isCompoundProcedure() const {
    return getType() == COMP_PROC_TY;
}

bool Scheme::SchemeObject::isEnvironment() const {
    return getType() == ENV_TY;
}
