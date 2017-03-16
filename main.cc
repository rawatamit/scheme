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

void repl(Scheme::Reader& reader, std::shared_ptr<Scheme::Environment> env) {
    std::cout << "> ";
    auto obj = reader.read();
    
    // FIXME: fix the try catch block
    if (obj) {
        try {
            auto val = eval(obj, env);
            if (val) {
                print(val, std::cout);
                std::cout << '\n';
            }
        } catch (std::runtime_error const* e) {
            std::cerr << e->what() << '\n';
        } catch (...) {
            std::cerr << "something bad happened\n";
        }
    }
}

void install_primitives(std::shared_ptr<Scheme::Environment> env) {
    INSTALL_PROC(env, "null?", Scheme::is_null_builtin);
    INSTALL_PROC(env, "boolean?", Scheme::is_boolean_builtin);
    INSTALL_PROC(env, "symbol?", Scheme::is_symbol_builtin);
    INSTALL_PROC(env, "integer?", Scheme::is_fixnum_builtin);
    INSTALL_PROC(env, "char?", Scheme::is_char_builtin);
    INSTALL_PROC(env, "string?", Scheme::is_string_builtin);
    INSTALL_PROC(env, "pair?", Scheme::is_pair_builtin);
    INSTALL_PROC(env, "procedure?", Scheme::is_procedure_builtin);

    //INSTALL_PROC(env, "char->integer", Scheme::char_to_fixnum_builtin);

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
    INSTALL_PROC(env, "list", Scheme::list_builtin);
}

int main(int argc, char **argv) {
    Scheme::Reader reader(std::cin, "<stdin>");
    auto env = std::make_shared<Scheme::Environment>();

    // install primitive procedures
    install_primitives(env);

    try {
        while (not reader.isEof()) {
            repl(reader, env);
        }
    } catch (Scheme::ReaderException const* e) {
        std::cerr << e->what() << '\n';
    } catch (...) {
        std::cerr << "exiting\n";
    }
    
    return 0;
}
