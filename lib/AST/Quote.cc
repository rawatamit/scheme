#include "AST/Quote.h"

Scheme::Quote::Quote(SchemeObject* obj) :
    Scheme::SchemeObject(Scheme::SchemeObject::QUOTE_TY), obj_(obj)
{
}

Scheme::Quote::~Quote() {
}

Scheme::SchemeObject* Scheme::Quote::getTextOfQuotation() const {
    return obj_;
}
