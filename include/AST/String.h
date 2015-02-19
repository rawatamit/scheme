#ifndef STRING_H
#define STRING_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class String : public SchemeObject {
private:
    Token* value_;
public:
    String(Token* value);
    virtual ~String();
    Token* getValue() const;
};
} // namespace Scheme

#endif
