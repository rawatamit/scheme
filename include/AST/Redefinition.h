#ifndef REDEFINITION_H
#define REDEFINITION_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Redefinition : public SchemeObject {
private:
    SchemeObject* var_;
    SchemeObject* val_;
public:
    Redefinition(SchemeObject* var, SchemeObject* val);
    virtual ~Redefinition();
    SchemeObject const* getVariable() const;
    SchemeObject const* getValue() const;
};
} // namespace Scheme

#endif
