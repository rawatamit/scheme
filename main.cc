#include "AST/SchemeObject.h"
#include "AST/Symbol.h"
#include "AST/Procedure.h"
#include "interp/Environment.h"
#include "interp/Reader.h"
#include "interp/Eval.h"
#include "interp/Print.h"
#include "interp/ReaderException.h"
#include <iostream>

#define INSTALL_PROC(ENV, NAME, DEFINITION) do { \
        (ENV)->getCurrentFrame()->define((NAME), std::make_shared<Scheme::Procedure>(DEFINITION)); \
    } while(0)

void install_primitives(std::shared_ptr<Scheme::Environment> env) {
    INSTALL_PROC(env, "apply", Scheme::apply_builtin);
    INSTALL_PROC(env, "eval", Scheme::eval_builtin);
    INSTALL_PROC(env, "interaction-environment", Scheme::interaction_env_builtin);
    INSTALL_PROC(env, "null-environment", Scheme::null_env_builtin);
    INSTALL_PROC(env, "environment", Scheme::env_builtin);

    INSTALL_PROC(env, "null?", Scheme::is_null_builtin);
    INSTALL_PROC(env, "boolean?", Scheme::is_boolean_builtin);
    INSTALL_PROC(env, "symbol?", Scheme::is_symbol_builtin);
    INSTALL_PROC(env, "integer?", Scheme::is_fixnum_builtin);
    INSTALL_PROC(env, "char?", Scheme::is_char_builtin);
    INSTALL_PROC(env, "string?", Scheme::is_string_builtin);
    INSTALL_PROC(env, "pair?", Scheme::is_pair_builtin);
    INSTALL_PROC(env, "procedure?", Scheme::is_procedure_builtin);

    INSTALL_PROC(env, "char->integer", Scheme::char_to_fixnum_builtin);
    INSTALL_PROC(env, "integer->char", Scheme::fixnum_to_char_builtin);
    INSTALL_PROC(env, "symbol->string", Scheme::symbol_to_string_builtin);
    INSTALL_PROC(env, "string->symbol", Scheme::string_to_symbol_builtin);
    INSTALL_PROC(env, "number->string", Scheme::fixnum_to_string_builtin);
    INSTALL_PROC(env, "string->number", Scheme::string_to_fixnum_builtin);

    INSTALL_PROC(env, "+", Scheme::add_builtin);
    INSTALL_PROC(env, "-", Scheme::sub_builtin);
    INSTALL_PROC(env, "*", Scheme::mul_builtin);
    INSTALL_PROC(env, "quotient", Scheme::quotient_builtin);
    INSTALL_PROC(env, "remainder", Scheme::remainder_builtin);
    INSTALL_PROC(env, "=", Scheme::equal_fixnum_builtin);
    INSTALL_PROC(env, "<", Scheme::lt_fixnum_builtin);
    INSTALL_PROC(env, ">", Scheme::gt_fixnum_builtin);

    INSTALL_PROC(env, "cons", Scheme::cons_builtin);
    INSTALL_PROC(env, "car", Scheme::car_builtin);
    INSTALL_PROC(env, "cdr", Scheme::cdr_builtin);
    INSTALL_PROC(env, "set-car!", Scheme::set_car_builtin);
    INSTALL_PROC(env, "set-cdr!", Scheme::set_cdr_builtin);
    INSTALL_PROC(env, "list", Scheme::list_builtin);

    INSTALL_PROC(env, "eq?", Scheme::eq_builtin);

    // input/output routines
    INSTALL_PROC(env, "load", Scheme::load_builtin);
    INSTALL_PROC(env, "read", Scheme::read_builtin);
    INSTALL_PROC(env, "read-char", Scheme::read_char_builtin);
    INSTALL_PROC(env, "peek-char", Scheme::peek_char_builtin);
    INSTALL_PROC(env, "eof-object?", Scheme::eof_object_builtin);
    INSTALL_PROC(env, "input-port?", Scheme::input_port_builtin);
    INSTALL_PROC(env, "open-input-port", Scheme::open_input_port_builtin);
    INSTALL_PROC(env, "close-input-port", Scheme::close_input_port_builtin);

    INSTALL_PROC(env, "write", Scheme::write_builtin);
    INSTALL_PROC(env, "write-char", Scheme::write_char_builtin);
    INSTALL_PROC(env, "output-port?", Scheme::output_port_builtin);
    INSTALL_PROC(env, "open-output-port", Scheme::open_output_port_builtin);
    INSTALL_PROC(env, "close-output-port", Scheme::close_output_port_builtin);
    INSTALL_PROC(env, "error", Scheme::error_builtin);
}

auto TOPLEVEL_ENV = std::make_shared<Scheme::Environment>();

int main(int argc, char **argv) {
    Scheme::Reader reader(stdin, "<stdin>");

    // install primitive procedures
    install_primitives(TOPLEVEL_ENV);

    try {
        while (not reader.isEof()) {
            std::cout << "> ";
            auto obj = reader.read();

            // FIXME: fix the try catch block
            if (obj) {
                try {
                    auto val = eval(obj, TOPLEVEL_ENV);
                    if (val) {
                        print(val, stdout);
                        std::cout << '\n';
                    }
                } catch (std::runtime_error& e) {
                    std::cerr << e.what() << '\n';
                } catch (...) {
                    std::cerr << "something bad happened\n";
                }
            }
        }
    } catch (Scheme::ReaderException& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "exiting\n";
    }
    
    return 0;
}
