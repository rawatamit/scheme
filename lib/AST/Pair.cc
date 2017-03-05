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
                   SchemeObject const* car, SchemeObject const* cdr) :
    SchemeObject(Scheme::SchemeObject::PAIR_TY),
    start_line_(start_line), start_col_(start_col),
    end_line_(end_line), end_col_(end_col),
    car_(car), cdr_(cdr)
{
}

Scheme::Pair::~Pair() {
}

Scheme::Pair const* Scheme::Pair::getEmptyList(int start_line, int start_col,
                                               int end_line, int end_col)
{
    return new Pair(
            Scheme::SchemeObject::EMPTYLIST_TY,
            start_line, start_col,
            end_line, end_col);
}

Scheme::SchemeObject const* Scheme::Pair::getCar() const {
    return car_;
}

Scheme::SchemeObject const* Scheme::Pair::getCdr() const {
    return cdr_;
}

Scheme::SchemeObject const* Scheme::Pair::getCaar() const {
    if (auto car = dynamic_cast<Scheme::Pair const*>(getCar())) {
        return car->getCar();
    }

    return nullptr;
}

Scheme::SchemeObject const* Scheme::Pair::getCdar() const {
    if (auto car = dynamic_cast<Scheme::Pair const*>(getCar())) {
        return car->getCdr();
    }

    return nullptr;
}

Scheme::SchemeObject const* Scheme::Pair::getCadr() const {
    if (auto cdr = dynamic_cast<Scheme::Pair const*>(getCdr())) {
        return cdr->getCar();
    }

    return nullptr;
}

Scheme::SchemeObject const* Scheme::Pair::getCddr() const {
    if (auto cdr = dynamic_cast<Scheme::Pair const*>(getCdr())) {
        return cdr->getCdr();
    }

    return nullptr;
}

Scheme::SchemeObject const* Scheme::Pair::getCaddr() const {
    if (auto cddr = dynamic_cast<Scheme::Pair const*>(getCddr())) {
        return cddr->getCar();
    }

    return nullptr;
}

Scheme::SchemeObject const* Scheme::Pair::getCdddr() const {
    if (auto cddr = dynamic_cast<Scheme::Pair const*>(getCddr())) {
        return cddr->getCdr();
    }

    return nullptr;
}

Scheme::SchemeObject const* Scheme::Pair::getCadddr() const {
    if (auto cdddr = dynamic_cast<Scheme::Pair const*>(getCdddr())) {
        return cdddr->getCar();
    }

    return nullptr;
}
