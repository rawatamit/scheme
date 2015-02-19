#ifndef CHARACTER_H
#define CHARACTER_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Character : public SchemeObject {
private:
    Token* value_;
public:
    Character(Token* value);
    virtual ~Character();
    Token* getValue() const;
};
} // namespace Scheme

#endif
