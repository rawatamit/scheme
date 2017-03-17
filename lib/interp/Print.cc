#include "interp/Print.h"
#include "AST/Fixnum.h"
#include "AST/Boolean.h"
#include "AST/Character.h"
#include "AST/String.h"
#include "AST/Pair.h"
#include "AST/Symbol.h"

namespace {
void printFixnum(Scheme::SchemeObjectPtr obj, std::ostream& out) {
    out << std::dynamic_pointer_cast<Scheme::Fixnum>(obj)->getValue();
}

void printBoolean(Scheme::SchemeObjectPtr obj, std::ostream& out) {
    out << std::dynamic_pointer_cast<Scheme::Boolean>(obj)->getValue()->getText();
}

void printCharacter(Scheme::SchemeObjectPtr obj, std::ostream& out) {
    char val = std::dynamic_pointer_cast<Scheme::Character>(obj)->getValue();

    out << "#\\";
    switch (val) {
        case '\n': out << "newline"; break;
        case ' ': out << "space"; break;
        default: out << val; break;
    }
}

void printString(Scheme::SchemeObjectPtr obj, std::ostream& out) {
    auto str = std::dynamic_pointer_cast<Scheme::String>(obj);
    out << '"';
    for (char ch : str->getValue()->getText()) {
        switch (ch) {
        case '\\':
            out << "\\\\";
            break;
        case '\n':
            out << "\\n";
            break;
        case '"':
            out << "\\\"";
            break;
        default:
            out << ch;
            break;
        }
    }
    out << '"';
}

void printPair(Scheme::SchemeObjectPtr obj, std::ostream& out) {
    auto pair = std::dynamic_pointer_cast<Scheme::Pair>(obj);
    print(pair->getCar(), out); // print car
    Scheme::SchemeObjectPtr cdr = pair->getCdr();
    if (cdr->isPair()) {
        out << ' ';
        printPair(std::dynamic_pointer_cast<Scheme::Pair>(cdr), out);
    } else if (not cdr->isEmptyList()) {
        out << " . ";
        print(cdr, out);
    }
}

void printSymbol(Scheme::SchemeObjectPtr obj, std::ostream& out) {
    out << std::dynamic_pointer_cast<Scheme::Symbol>(obj)->getValue()->getText();
}
} // anonymous namespace

void Scheme::print(Scheme::SchemeObjectPtr obj, std::ostream& out) {
    switch (obj->getType()) {
    case Scheme::SchemeObject::FIXNUM_TY:
        printFixnum(obj, out);
        break;
    case Scheme::SchemeObject::BOOLEAN_TY:
        printBoolean(obj, out);
        break;
    case Scheme::SchemeObject::CHARACTER_TY:
        printCharacter(obj, out);
        break;
    case Scheme::SchemeObject::STRING_TY:
        printString(obj, out);
        break;
    case Scheme::SchemeObject::EMPTYLIST_TY:
        out << "()";
        //printPair(dynamic_cast<Scheme::Pair const*>(obj), out);
        break;
    case Scheme::SchemeObject::PAIR_TY:
        out << '(';
        printPair(obj, out);
        out << ')';
        break;
    case Scheme::SchemeObject::SYMBOL_TY:
        printSymbol(obj, out);
        break;
    case Scheme::SchemeObject::PROC_TY:
        out << "#<primitive>";
        break;
    default:
        break;
    }
}
