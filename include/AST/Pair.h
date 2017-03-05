#ifndef PAIR_H
#define PAIR_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Pair : public SchemeObject {
private:
    int start_line_, start_col_;
    int end_line_, end_col_;
    SchemeObject const* car_;
    SchemeObject const* cdr_;
    
    Pair(Scheme::SchemeObject::ObjectTy type,
         int start_line, int start_col,
         int end_line, int end_col);
public:
    Pair(int start_line, int start_col,
         int end_line, int end_col,
         SchemeObject const* car, SchemeObject const* cdr);
    virtual ~Pair();
    static Pair const* getEmptyList(int start_line, int start_col,
                                    int end_line, int end_col);
    SchemeObject const* getCar() const;
    SchemeObject const* getCdr() const;
    SchemeObject const* getCaar() const;
    SchemeObject const* getCdar() const;
    SchemeObject const* getCadr() const;
    SchemeObject const* getCddr() const;
    SchemeObject const* getCaddr() const;
    SchemeObject const* getCdddr() const;
    SchemeObject const* getCadddr() const;
};
} // namespace Scheme

#endif
