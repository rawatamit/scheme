#ifndef PAIR_H
#define PAIR_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Pair : public SchemeObject {
private:
    SchemeObjectPtr car_;
    SchemeObjectPtr cdr_;
    
    Pair(Scheme::SchemeObject::ObjectTy type);
public:
    Pair(SchemeObjectPtr car, SchemeObjectPtr cdr);
    virtual ~Pair();
    static SchemeObjectPtr getEmptyList();
    static SchemeObjectPtr cons(SchemeObjectPtr car, SchemeObjectPtr cdr);

    void setCar(SchemeObjectPtr ncar);
    void setCdr(SchemeObjectPtr ncdr);
    SchemeObjectPtr getCar() const;
    SchemeObjectPtr getCdr() const;
    SchemeObjectPtr getCaar() const;
    SchemeObjectPtr getCdar() const;
    SchemeObjectPtr getCadr() const;
    SchemeObjectPtr getCddr() const;
    SchemeObjectPtr getCdadr() const;
    SchemeObjectPtr getCaadr() const;
    SchemeObjectPtr getCaddr() const;
    SchemeObjectPtr getCdddr() const;
    SchemeObjectPtr getCadddr() const;
};
} // namespace Scheme

#endif
