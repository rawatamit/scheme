#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Boolean : public SchemeObject {
private:
    std::shared_ptr<Token> value_;
    static SchemeObjectPtr dummy_true;
    static SchemeObjectPtr dummy_false;
    Boolean(std::shared_ptr<Token> value);

public:
    virtual ~Boolean();
    static SchemeObjectPtr get(std::shared_ptr<Token> value);
    bool isTrue() const;
    bool isFalse() const;
    std::shared_ptr<Token> getValue() const;
    static SchemeObjectPtr getTrue();
    static SchemeObjectPtr getFalse();
};
} // namespace Scheme

#endif
