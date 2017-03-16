#include "AST/Pair.h"

Scheme::Pair::Pair(Scheme::SchemeObject::ObjectTy type,
                   int start_line, int start_col,
                   int end_line, int end_col) :
    SchemeObject(type),
    start_line_(start_line), start_col_(start_col),
    end_line_(end_line), end_col_(end_col)
{
}

Scheme::Pair::Pair(int start_line, int start_col, int end_line, int end_col,
                   SchemeObjectPtr car, SchemeObjectPtr cdr) :
    SchemeObject(Scheme::SchemeObject::PAIR_TY),
    start_line_(start_line), start_col_(start_col),
    end_line_(end_line), end_col_(end_col),
    car_(car), cdr_(cdr)
{
}

Scheme::Pair::~Pair() {
}

Scheme::SchemeObjectPtr Scheme::Pair::getEmptyList(int start_line, int start_col,
                                               int end_line, int end_col)
{
    return std::shared_ptr<Pair>(
            new Pair(
                Scheme::SchemeObject::EMPTYLIST_TY,
                start_line, start_col,
                end_line, end_col));
}

Scheme::SchemeObjectPtr Scheme::Pair::getCar() const {
    return car_;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCdr() const {
    return cdr_;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCaar() const {
    if (auto car = std::dynamic_pointer_cast<Scheme::Pair>(getCar())) {
        return car->getCar();
    }

    return nullptr;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCdar() const {
    if (auto car = std::dynamic_pointer_cast<Scheme::Pair>(getCar())) {
        return car->getCdr();
    }

    return nullptr;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCadr() const {
    if (auto cdr = std::dynamic_pointer_cast<Scheme::Pair>(getCdr())) {
        return cdr->getCar();
    }

    return nullptr;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCddr() const {
    if (auto cdr = std::dynamic_pointer_cast<Scheme::Pair>(getCdr())) {
        return cdr->getCdr();
    }

    return nullptr;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCaddr() const {
    if (auto cddr = std::dynamic_pointer_cast<Scheme::Pair>(getCddr())) {
        return cddr->getCar();
    }

    return nullptr;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCdddr() const {
    if (auto cddr = std::dynamic_pointer_cast<Scheme::Pair>(getCddr())) {
        return cddr->getCdr();
    }

    return nullptr;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCadddr() const {
    if (auto cdddr = std::dynamic_pointer_cast<Scheme::Pair>(getCdddr())) {
        return cdddr->getCar();
    }

    return nullptr;
}
