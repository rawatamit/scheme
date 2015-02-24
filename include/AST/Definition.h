#ifndef DEFINITION_H
#define DEFINITION_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Definition : public SchemeObject {
private:
    SchemeObject* var_;
    SchemeObject* val_;
public:
    Definition(SchemeObject* var, SchemeObject* val);
    virtual ~Definition();
    SchemeObject const* getVariable() const;
    SchemeObject const* getValue() const;
};
} // namespace Scheme

#endif
