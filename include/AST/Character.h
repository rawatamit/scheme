#ifndef CHARACTER_H
#define CHARACTER_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Character : public SchemeObject {
private:
    std::shared_ptr<Token> value_;
public:
    Character(std::shared_ptr<Token> value);
    virtual ~Character();
    std::shared_ptr<Token> getValue() const;
};
} // namespace Scheme

#endif
