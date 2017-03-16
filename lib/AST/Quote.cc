#include "AST/Quote.h"

Scheme::Quote::Quote(SchemeObjectPtr obj) :
    Scheme::SchemeObject(Scheme::SchemeObject::QUOTE_TY), obj_(obj)
{
}

Scheme::Quote::~Quote() {
}

Scheme::SchemeObjectPtr Scheme::Quote::getTextOfQuotation() const {
    return obj_;
}
