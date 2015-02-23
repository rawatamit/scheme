#include "interp/Eval.h"
#include "AST/Quote.h"
#include <stdexcept>

namespace {
bool isSelfEvaluating(Scheme::SchemeObject* obj) {
    return obj->isBoolean() or obj->isFixnum()
        or obj->isCharacter() or obj->isString();
}
} // anonymous namespace

Scheme::SchemeObject* Scheme::eval(Scheme::SchemeObject* obj) {
    if (isSelfEvaluating(obj)) {
        return obj;
    } else if (obj->isQuote()) {
        return dynamic_cast<Scheme::Quote*>(obj)->getTextOfQuotation();
    }
    throw std::exception();
}
