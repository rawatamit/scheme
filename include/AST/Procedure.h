#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "AST/SchemeObject.h"
#include <cstdio>
#include <cstring>

namespace Scheme {
    class Procedure : public SchemeObject {
    public:
        typedef SchemeObject const* (*FunctionType)(SchemeObject const* arguments);

    private:
        FunctionType f_;

    public:
        Procedure(FunctionType f);
        virtual ~Procedure();

        FunctionType getFunction() const;
    };

#define DECLARE_PROC(NAME) SchemeObject const* NAME(SchemeObject const* arguments);

    DECLARE_PROC(is_null_builtin);
    DECLARE_PROC(is_boolean_builtin);
    DECLARE_PROC(is_symbol_builtin);
    DECLARE_PROC(is_fixnum_builtin);
    DECLARE_PROC(is_char_builtin);
    DECLARE_PROC(is_string_builtin);
    DECLARE_PROC(is_pair_builtin);
    DECLARE_PROC(is_procedure_builtin);

    //DECLARE_PROC(char_to_fixnum_builtin);

    DECLARE_PROC(add_builtin);
    DECLARE_PROC(sub_builtin);
    DECLARE_PROC(mul_builtin);
    DECLARE_PROC(quotient_builtin);
    DECLARE_PROC(remainder_builtin);
    DECLARE_PROC(equal_fixnum_builtin);
    DECLARE_PROC(lt_fixnum_builtin);
    DECLARE_PROC(gt_fixnum_builtin);

    DECLARE_PROC(cons_builtin);
    DECLARE_PROC(car_builtin);
    DECLARE_PROC(cdr_builtin);
    DECLARE_PROC(list_builtin);
} // namespace Scheme

#endif // PROCEDURE_H
