#ifndef FIXNUM_H
#define FIXNUM_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Fixnum : public SchemeObject {
private:
    long value_;

public:
    Fixnum(long value);
    virtual ~Fixnum();
    long getValue() const;
};
} // namespace Scheme

#endif
