#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "AST/SchemeObject.h"
#include <cstdio>
#include <cstring>

namespace Scheme {
    class Procedure : public SchemeObject {
    public:
        typedef SchemeObjectPtr (*FunctionType)(SchemeObjectPtr arguments);

    private:
        FunctionType f_;

    public:
        Procedure(FunctionType f);
        virtual ~Procedure();

        FunctionType getFunction() const;
    };

#define DECLARE_PROC(NAME) SchemeObjectPtr NAME(SchemeObjectPtr arguments);

    DECLARE_PROC(apply_builtin);
    DECLARE_PROC(eval_builtin);
    DECLARE_PROC(interaction_env_builtin);
    DECLARE_PROC(null_env_builtin);
    DECLARE_PROC(env_builtin);

    DECLARE_PROC(is_null_builtin);
    DECLARE_PROC(is_boolean_builtin);
    DECLARE_PROC(is_symbol_builtin);
    DECLARE_PROC(is_fixnum_builtin);
    DECLARE_PROC(is_char_builtin);
    DECLARE_PROC(is_string_builtin);
    DECLARE_PROC(is_pair_builtin);
    DECLARE_PROC(is_procedure_builtin);

    DECLARE_PROC(char_to_fixnum_builtin);
    DECLARE_PROC(fixnum_to_char_builtin);
    DECLARE_PROC(symbol_to_string_builtin);
    DECLARE_PROC(string_to_symbol_builtin);
    DECLARE_PROC(fixnum_to_string_builtin);
    DECLARE_PROC(string_to_fixnum_builtin);

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
    DECLARE_PROC(set_car_builtin);
    DECLARE_PROC(set_cdr_builtin);
    DECLARE_PROC(list_builtin);

    DECLARE_PROC(eq_builtin);

    DECLARE_PROC(load_builtin);
    DECLARE_PROC(read_builtin);
    DECLARE_PROC(read_char_builtin);
    DECLARE_PROC(peek_char_builtin);
    DECLARE_PROC(eof_object_builtin);
    DECLARE_PROC(input_port_builtin);
    DECLARE_PROC(open_input_port_builtin);
    DECLARE_PROC(close_input_port_builtin);

    DECLARE_PROC(write_builtin);
    DECLARE_PROC(write_char_builtin);
    DECLARE_PROC(output_port_builtin);
    DECLARE_PROC(open_output_port_builtin);
    DECLARE_PROC(close_output_port_builtin);
    DECLARE_PROC(error_builtin);
} // namespace Scheme

#endif // PROCEDURE_H
