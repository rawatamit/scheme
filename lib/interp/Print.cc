#include "interp/Print.h"
#include "AST/Fixnum.h"
#include "AST/Boolean.h"
#include "AST/Character.h"
#include "AST/String.h"
#include "AST/Pair.h"
#include "AST/Symbol.h"

namespace {
void printFixnum(Scheme::Fixnum const* obj, std::ostream& out) {
    out << obj->getValue()->getText();
}

void printBoolean(Scheme::Boolean const* obj, std::ostream& out) {
    out << obj->getValue()->getText();
}

void printCharacter(Scheme::Character const* obj, std::ostream& out) {
    out << obj->getValue()->getText();
}

void printString(Scheme::String const* obj, std::ostream& out) {
    out << '"';
    for (char ch : obj->getValue()->getText()) {
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

void printPair(Scheme::Pair const* obj, std::ostream& out) {
    print(obj->getCar(), out); // print car
    Scheme::SchemeObject const* cdr = obj->getCdr();
    if (cdr->isPair()) {
        out << ' ';
        printPair(dynamic_cast<Scheme::Pair const*>(cdr), out);
    } else if (not cdr->isEmptyList()) {
        out << " . ";
        print(cdr, out);
    }
}

void printSymbol(Scheme::Symbol const* obj, std::ostream& out) {
    out << obj->getValue()->getText();
}
} // anonymous namespace

void Scheme::print(Scheme::SchemeObject const* obj, std::ostream& out) {
    switch (obj->getType()) {
    case Scheme::SchemeObject::FIXNUM_TY:
        printFixnum(dynamic_cast<Scheme::Fixnum const*>(obj), out);
        break;
    case Scheme::SchemeObject::BOOLEAN_TY:
        printBoolean(dynamic_cast<Scheme::Boolean const*>(obj), out);
        break;
    case Scheme::SchemeObject::CHARACTER_TY:
        printCharacter(dynamic_cast<Scheme::Character const*>(obj), out);
        break;
    case Scheme::SchemeObject::STRING_TY:
        printString(dynamic_cast<Scheme::String const*>(obj), out);
        break;
    case Scheme::SchemeObject::EMPTYLIST_TY:
        out << "()";
        //printPair(dynamic_cast<Scheme::Pair const*>(obj), out);
        break;
    case Scheme::SchemeObject::PAIR_TY:
        out << '(';
        printPair(dynamic_cast<Scheme::Pair const*>(obj), out);
        out << ')';
        break;
    case Scheme::SchemeObject::SYMBOL_TY:
        printSymbol(dynamic_cast<Scheme::Symbol const*>(obj), out);
        break;
    case Scheme::SchemeObject::PROC_TY:
        out << "<primitive>";
        break;
    default:
        break;
    }
}
