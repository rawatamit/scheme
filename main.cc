#include "AST/SchemeObject.h"
#include "interp/Environment.h"
#include "interp/Reader.h"
#include "interp/Eval.h"
#include "interp/Print.h"
#include <iostream>
#include <stdexcept>

void repl(Scheme::Reader& reader, Scheme::Environment* env) {
    std::cout << "> ";
    Scheme::SchemeObject const* obj = reader.read();
    
    // FIXME: fix the try catch block
    if (obj) {
        try {
            Scheme::SchemeObject const* val = eval(obj, env);
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

int main(int argc, char **argv) {
    Scheme::Reader reader(std::cin, "<stdin>");
    Scheme::Environment* env = new Scheme::Environment();
    
    while (not reader.isEof()) {
        repl(reader, env);
    }
    
    std::cout << '\n';
    return 0;
}
