#ifndef REDEFINITION_H
#define REDEFINITION_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Redefinition : public SchemeObject {
private:
    SchemeObjectPtr var_;
    SchemeObjectPtr val_;

public:
    Redefinition(SchemeObjectPtr var, SchemeObjectPtr val);
    virtual ~Redefinition();
    SchemeObjectPtr getVariable() const;
    SchemeObjectPtr getValue() const;
};
} // namespace Scheme

#endif
