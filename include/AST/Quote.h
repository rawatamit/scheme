#ifndef QUOTE_H
#define QUOTE_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Quote : public SchemeObject {
private:
    SchemeObjectPtr obj_;
public:
    Quote(SchemeObjectPtr obj);
    virtual ~Quote();
    SchemeObjectPtr getTextOfQuotation() const;
};
} // namespace Scheme

#endif
