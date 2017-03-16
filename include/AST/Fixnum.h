#ifndef FIXNUM_H
#define FIXNUM_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Fixnum : public SchemeObject {
private:
    std::shared_ptr<Token> value_;
public:
    Fixnum(std::shared_ptr<Token> value);
    virtual ~Fixnum();
    std::shared_ptr<Token> getValue() const;
};
} // namespace Scheme

#endif
