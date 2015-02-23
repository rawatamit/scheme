#ifndef QUOTE_H
#define QUOTE_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Quote : public SchemeObject {
private:
    SchemeObject* obj_;
public:
    Quote(SchemeObject* obj);
    virtual ~Quote();
    SchemeObject* getTextOfQuotation() const;
};
} // namespace Scheme

#endif
