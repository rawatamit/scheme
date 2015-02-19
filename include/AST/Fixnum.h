#ifndef FIXNUM_H
#define FIXNUM_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Fixnum : public SchemeObject {
private:
    Token* value_;
public:
    Fixnum(Token* value);
    virtual ~Fixnum();
    Token* getValue() const;
};
} // namespace Scheme

#endif
