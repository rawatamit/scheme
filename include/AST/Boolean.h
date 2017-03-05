#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Boolean : public SchemeObject {
private:
    Token* value_;
    static Boolean const* dummy_true;
    static Boolean const* dummy_false;
    Boolean(Token* value);

public:
    virtual ~Boolean();
    static Boolean const* get(Token* value);
    bool isTrue() const;
    bool isFalse() const;
    Token* getValue() const;
    static Scheme::Boolean const* getTrue();
    static Scheme::Boolean const* getFalse();
};
} // namespace Scheme

#endif
