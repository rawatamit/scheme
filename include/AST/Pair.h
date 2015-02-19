#ifndef PAIR_H
#define PAIR_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"

namespace Scheme {
class Pair : public SchemeObject {
private:
    int start_line_, start_col_;
    int end_line_, end_col_;
    SchemeObject* car_;
    SchemeObject* cdr_;
    
    Pair(Scheme::SchemeObject::ObjectTy type,
         int start_line, int start_col,
         int end_line, int end_col);
public:
    Pair(int start_line, int start_col,
         int end_line, int end_col,
         SchemeObject* car, SchemeObject* cdr);
    virtual ~Pair();
    static Pair* getEmptyList(int start_line, int start_col,
                              int end_line, int end_col);
    SchemeObject const* getCar() const;
    SchemeObject const* getCdr() const;
    SchemeObject* getCar();
    SchemeObject* getCdr();
    void setCar(SchemeObject* car);
    void setCdr(SchemeObject* cdr);
};
} // namespace Scheme

#endif
