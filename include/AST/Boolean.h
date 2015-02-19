#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Boolean : public SchemeObject {
private:
    Token* value_;
    bool isTrue_;
    Boolean(Token* value, bool isTrue);
public:
    virtual ~Boolean();
    static Boolean* get(Token* value);
    bool isTrue() const;
    bool isFalse() const;
    Token* getValue() const;
};
} // namespace Scheme

#endif
