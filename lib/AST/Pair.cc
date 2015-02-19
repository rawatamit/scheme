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
                   SchemeObject* car, SchemeObject* cdr) :
    SchemeObject(Scheme::SchemeObject::PAIR_TY),
    start_line_(start_line), start_col_(start_col),
    end_line_(end_line), end_col_(end_col),
    car_(car), cdr_(cdr)
{
}

Scheme::Pair::~Pair() {
}

Scheme::Pair* Scheme::Pair::getEmptyList(int start_line, int start_col,
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

Scheme::SchemeObject* Scheme::Pair::getCar() {
    return car_;
}

Scheme::SchemeObject* Scheme::Pair::getCdr() {
    return cdr_;
}

void Scheme::Pair::setCar(Scheme::SchemeObject* car) {
    car_ = car;
}

void Scheme::Pair::setCdr(Scheme::SchemeObject* cdr) {
    cdr_ = cdr;
}
