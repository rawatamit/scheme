#include "AST/Pair.h"

Scheme::Pair::Pair(Scheme::SchemeObject::ObjectTy type) :
    SchemeObject(type)
{
}

Scheme::Pair::Pair(SchemeObjectPtr car, SchemeObjectPtr cdr) :
    SchemeObject(Scheme::SchemeObject::PAIR_TY),
    car_(car), cdr_(cdr)
{
}

Scheme::Pair::~Pair() {
}

Scheme::SchemeObjectPtr Scheme::Pair::getEmptyList()
{
    return std::shared_ptr<Pair>(new Pair(Scheme::SchemeObject::EMPTYLIST_TY));
}

void Scheme::Pair::setCar(Scheme::SchemeObjectPtr ncar) {
    car_ = ncar;
}

void Scheme::Pair::setCdr(Scheme::SchemeObjectPtr ncdr) {
    cdr_ = ncdr;
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

Scheme::SchemeObjectPtr Scheme::Pair::getCdadr() const {
    if (auto cadr = std::dynamic_pointer_cast<Scheme::Pair>(getCadr())) {
        return cadr->getCdr();
    }

    return nullptr;
}

Scheme::SchemeObjectPtr Scheme::Pair::getCaadr() const {
    if (auto cadr = std::dynamic_pointer_cast<Scheme::Pair>(getCadr())) {
        return cadr->getCar();
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
