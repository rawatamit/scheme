#include "AST/SchemeObject.h"
#include "interp/Reader.h"
#include "interp/Eval.h"
#include "interp/Print.h"
#include "interp/ReaderException.h"
#include <iostream>

void repl(Scheme::Reader& reader) {
    std::cout << "> ";
    Scheme::SchemeObject* obj = reader.read();
    if (obj) {
        print(eval(obj), std::cout);
        std::cout << '\n';
    }
}

int main(int argc, char **argv) {
    Scheme::Reader reader(std::cin, "<stdin>");
    
    while (not reader.isEof()) {
        try {
            repl(reader);
        } catch (Scheme::ReaderException const* e) {
            std::cout << e->what() << '\n';
        }
    }
    
    std::cout << '\n';
    return 0;
}
