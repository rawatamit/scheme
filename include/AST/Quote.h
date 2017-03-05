#ifndef QUOTE_H
#define QUOTE_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Quote : public SchemeObject {
private:
    SchemeObject const* obj_;
public:
    Quote(SchemeObject const* obj);
    virtual ~Quote();
    SchemeObject const* getTextOfQuotation() const;
};
} // namespace Scheme

#endif
