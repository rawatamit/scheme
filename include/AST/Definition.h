#ifndef DEFINITION_H
#define DEFINITION_H

#include "AST/SchemeObject.h"

namespace Scheme {
class Definition : public SchemeObject {
private:
    SchemeObject const* var_;
    SchemeObject const* val_;
public:
    Definition(SchemeObject const* var, SchemeObject const* val);
    virtual ~Definition();
    SchemeObject const* getVariable() const;
    SchemeObject const* getValue() const;
};
} // namespace Scheme

#endif
