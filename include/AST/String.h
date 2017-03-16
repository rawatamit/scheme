#ifndef STRING_H
#define STRING_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class String : public SchemeObject {
private:
    std::shared_ptr<Token> value_;
public:
    String(std::shared_ptr<Token> value);
    virtual ~String();
    std::shared_ptr<Token> getValue() const;
};
} // namespace Scheme

#endif
