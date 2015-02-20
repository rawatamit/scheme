#include "interp/Print.h"
#include "AST/Fixnum.h"
#include "AST/Boolean.h"
#include "AST/Character.h"
#include "AST/String.h"
#include "AST/Pair.h"
#include "AST/Symbol.h"

namespace {
void printFixnum(Scheme::Fixnum* obj, std::ostream& out) {
    out << obj->getValue()->getText();
}

void printBoolean(Scheme::Boolean* obj, std::ostream& out) {
    out << obj->getValue()->getText();
}

void printCharacter(Scheme::Character* obj, std::ostream& out) {
    out << obj->getValue()->getText();
}

void printString(Scheme::String* obj, std::ostream& out) {
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

void printPair(Scheme::Pair* obj, std::ostream& out) {
    print(obj->getCar(), out); // print car
    Scheme::SchemeObject* cdr = obj->getCdr();
    if (cdr->isPair()) {
        out << ' ';
        printPair(dynamic_cast<Scheme::Pair*>(cdr), out);
    } else if (not cdr->isEmptyList()) {
        out << " . ";
        print(cdr, out);
    }
}

void printSymbol(Scheme::Symbol* obj, std::ostream& out) {
    out << obj->getValue()->getText();
}
} // anonymous namespace

void Scheme::print(Scheme::SchemeObject* obj, std::ostream& out) {
    switch (obj->getType()) {
    case Scheme::SchemeObject::FIXNUM_TY:
        printFixnum(dynamic_cast<Scheme::Fixnum*>(obj), out);
        break;
    case Scheme::SchemeObject::BOOLEAN_TY:
        printBoolean(dynamic_cast<Scheme::Boolean*>(obj), out);
        break;
    case Scheme::SchemeObject::CHARACTER_TY:
        printCharacter(dynamic_cast<Scheme::Character*>(obj), out);
        break;
    case Scheme::SchemeObject::STRING_TY:
        printString(dynamic_cast<Scheme::String*>(obj), out);
        break;
    case Scheme::SchemeObject::EMPTYLIST_TY:
        printPair(dynamic_cast<Scheme::Pair*>(obj), out);
        break;
    case Scheme::SchemeObject::PAIR_TY:
        out << '(';
        printPair(dynamic_cast<Scheme::Pair*>(obj), out);
        out << ')';
        break;
    case Scheme::SchemeObject::SYMBOL_TY:
        printSymbol(dynamic_cast<Scheme::Symbol*>(obj), out);
    default:
        break;
    }
}
