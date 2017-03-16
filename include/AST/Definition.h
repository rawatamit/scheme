#ifndef DEFINITION_H
#define DEFINITION_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Definition : public SchemeObject {
private:
    SchemeObjectPtr var_;
    SchemeObjectPtr val_;
public:
    Definition(SchemeObjectPtr var, SchemeObjectPtr val);
    virtual ~Definition();
    SchemeObjectPtr getVariable() const;
    SchemeObjectPtr getValue() const;
};
} // namespace Scheme

#endif
