#ifndef REDEFINITION_H
#define REDEFINITION_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Redefinition : public SchemeObject {
private:
    SchemeObject const* var_;
    SchemeObject const* val_;

public:
    Redefinition(SchemeObject const* var, SchemeObject const* val);
    virtual ~Redefinition();
    SchemeObject const* getVariable() const;
    SchemeObject const* getValue() const;
};
} // namespace Scheme

#endif
