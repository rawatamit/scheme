#ifndef PAIR_H
#define PAIR_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Pair : public SchemeObject {
private:
    int start_line_, start_col_;
    int end_line_, end_col_;
    SchemeObjectPtr car_;
    SchemeObjectPtr cdr_;
    
    Pair(Scheme::SchemeObject::ObjectTy type,
         int start_line, int start_col,
         int end_line, int end_col);
public:
    Pair(int start_line, int start_col,
         int end_line, int end_col,
         SchemeObjectPtr car, SchemeObjectPtr cdr);
    virtual ~Pair();
    static SchemeObjectPtr getEmptyList(int start_line, int start_col,
                                              int end_line, int end_col);
    SchemeObjectPtr getCar() const;
    SchemeObjectPtr getCdr() const;
    SchemeObjectPtr getCaar() const;
    SchemeObjectPtr getCdar() const;
    SchemeObjectPtr getCadr() const;
    SchemeObjectPtr getCddr() const;
    SchemeObjectPtr getCaddr() const;
    SchemeObjectPtr getCdddr() const;
    SchemeObjectPtr getCadddr() const;
};
} // namespace Scheme

#endif
