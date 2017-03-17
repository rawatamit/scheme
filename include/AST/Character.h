#ifndef CHARACTER_H
#define CHARACTER_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Character : public SchemeObject {
private:
    char value_;
public:
    Character(char value);
    virtual ~Character();
    char getValue() const;
};
} // namespace Scheme

#endif
