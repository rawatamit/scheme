#include "interp/Print.h"
#include "AST/Fixnum.h"
#include "AST/Boolean.h"
#include "AST/Character.h"
#include "AST/String.h"
#include "AST/Pair.h"
#include "AST/Symbol.h"

namespace {
void printFixnum(Scheme::SchemeObjectPtr obj, FILE* out) {
    fprintf(out, "%ld", std::dynamic_pointer_cast<Scheme::Fixnum>(obj)->getValue());
}

void printBoolean(Scheme::SchemeObjectPtr obj, FILE* out) {
    fprintf(out, "%s", std::dynamic_pointer_cast<Scheme::Boolean>(obj)->getValue()->getText().c_str());
}

void printCharacter(Scheme::SchemeObjectPtr obj, FILE* out) {
    char val = std::dynamic_pointer_cast<Scheme::Character>(obj)->getValue();

    fprintf(out, "#\\");
    switch (val) {
        case '\n': fprintf(out, "%s", "newline"); break;
        case ' ': fprintf(out, "%s", "space"); break;
        default: fprintf(out, "%c", val); break;
    }
}

void printString(Scheme::SchemeObjectPtr obj, FILE* out) {
    auto str = std::dynamic_pointer_cast<Scheme::String>(obj);

    fprintf(out, "%c", '"');
    for (char ch : str->getValue()->getText()) {
        switch (ch) {
        case '\\':
            fprintf(out, "%s", "\\\\");
            break;
        case '\n':
            fprintf(out, "%s", "\\n");
            break;
        case '"':
            fprintf(out, "%s", "\\\"");
            break;
        default:
            fprintf(out, "%c", ch);
            break;
        }
    }

    fprintf(out, "%c", '"');
}

void printPair(Scheme::SchemeObjectPtr obj, FILE* out) {
    auto pair = std::dynamic_pointer_cast<Scheme::Pair>(obj);
    print(pair->getCar(), out); // print car
    Scheme::SchemeObjectPtr cdr = pair->getCdr();
    if (cdr->isPair()) {
        fprintf(out, "%c", ' ');
        printPair(std::dynamic_pointer_cast<Scheme::Pair>(cdr), out);
    } else if (not cdr->isEmptyList()) {
        fprintf(out, "%s", " . ");
        print(cdr, out);
    }
}

void printSymbol(Scheme::SchemeObjectPtr obj, FILE* out) {
    fprintf(out, "%s", std::dynamic_pointer_cast<Scheme::Symbol>(obj)->getValue()->getText().c_str());
}
} // anonymous namespace

void Scheme::print(Scheme::SchemeObjectPtr obj, FILE* out) {
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
        fprintf(out, "%s", "()");
        break;
    case Scheme::SchemeObject::PAIR_TY:
        fprintf(out, "%c", '(');
        printPair(obj, out);
        fprintf(out, "%c", ')');
        break;
    case Scheme::SchemeObject::SYMBOL_TY:
        printSymbol(obj, out);
        break;
    case Scheme::SchemeObject::PROC_TY:
        fprintf(out, "%s", "#<primitive>");
        break;
    case Scheme::SchemeObject::COMP_PROC_TY:
        fprintf(out, "%s", "#<procedure>");
        break;
    case Scheme::SchemeObject::ENV_TY:
        fprintf(out, "%s", "#<environment>");
        break;
    case Scheme::SchemeObject::INPUT_PORT_TY:
        fprintf(out, "%s", "#<input-port>");
        break;
    case Scheme::SchemeObject::OUTPUT_PORT_TY:
        fprintf(out, "%s", "#<output-port>");
        break;
    default:
        fprintf(out, "%s:%d", "#<unknown-object>", obj->getType());
        break;
    }
}
