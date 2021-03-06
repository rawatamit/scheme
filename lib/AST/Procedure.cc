#include "AST/Procedure.h"
#include "AST/Pair.h"
#include "AST/Fixnum.h"
#include "AST/Boolean.h"
#include "AST/Character.h"
#include "AST/Symbol.h"
#include "AST/String.h"
#include "AST/SchemeEnvironment.h"
#include "AST/InputPort.h"
#include "AST/OutputPort.h"
#include "interp/Reader.h"
#include "interp/Print.h"
#include "interp/Eval.h"
#include "interp/EvalException.h"
#include "interp/ReaderException.h"

Scheme::Procedure::Procedure(Scheme::Procedure::FunctionType f) :
    Scheme::SchemeObject(Scheme::SchemeObject::PROC_TY),
    f_(f)
{}

Scheme::Procedure::~Procedure()
{}

Scheme::Procedure::FunctionType Scheme::Procedure::getFunction() const
{
    return f_;
}

// builtins
Scheme::SchemeObjectPtr Scheme::apply_builtin(SchemeObjectPtr) {
    throw EvalException("error: apply builtin body directly called");
}

Scheme::SchemeObjectPtr Scheme::eval_builtin(SchemeObjectPtr) {
    throw EvalException("error: eval builtin body directly called");
}

extern std::shared_ptr<Scheme::Environment> TOPLEVEL_ENV;

Scheme::SchemeObjectPtr Scheme::interaction_env_builtin(SchemeObjectPtr) {
    return std::make_shared<SchemeEnvironment>(TOPLEVEL_ENV);
}

Scheme::SchemeObjectPtr Scheme::null_env_builtin(SchemeObjectPtr) {
    return std::make_shared<SchemeEnvironment>();
}

extern void install_primitives(std::shared_ptr<Scheme::Environment> env);

Scheme::SchemeObjectPtr Scheme::env_builtin(SchemeObjectPtr) {
    auto env = std::make_shared<SchemeEnvironment>();
    install_primitives(env->getEnvironment());
    return env;
}

#define TYPE_PREDICATE(NAME, PREDICATE) Scheme::SchemeObjectPtr Scheme::NAME(Scheme::SchemeObjectPtr arguments) { \
    return std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar()->PREDICATE() ? Scheme::Boolean::getTrue() \
                                                                                      : Scheme::Boolean::getFalse(); \
}

TYPE_PREDICATE(is_null_builtin, isEmptyList);
TYPE_PREDICATE(is_boolean_builtin, isBoolean);
TYPE_PREDICATE(is_symbol_builtin, isSymbol);
TYPE_PREDICATE(is_fixnum_builtin, isFixnum);
TYPE_PREDICATE(is_char_builtin, isCharacter);
TYPE_PREDICATE(is_string_builtin, isString);
TYPE_PREDICATE(is_pair_builtin, isPair);
TYPE_PREDICATE(is_procedure_builtin, isProcedure);

#define TYPE_CONVERSION(NAME, FROM_CLASS, TO_CLASS) \
Scheme::SchemeObjectPtr Scheme::NAME(Scheme::SchemeObjectPtr arguments) { \
    return std::make_shared<TO_CLASS>(std::dynamic_pointer_cast<FROM_CLASS>( \
                std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue()); \
}

TYPE_CONVERSION(char_to_fixnum_builtin, Scheme::Character, Scheme::Fixnum);
TYPE_CONVERSION(fixnum_to_char_builtin, Scheme::Fixnum, Scheme::Character);
TYPE_CONVERSION(symbol_to_string_builtin, Scheme::Symbol, Scheme::String);
TYPE_CONVERSION(string_to_symbol_builtin, Scheme::String, Scheme::Symbol);

Scheme::SchemeObjectPtr Scheme::fixnum_to_string_builtin(SchemeObjectPtr arguments) {
    char buf[128];
    auto val = std::dynamic_pointer_cast<Scheme::Fixnum>(
                    std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue();
    snprintf(buf, sizeof(buf), "%ld", val);
    return std::make_shared<Scheme::String>(
                std::make_shared<Token>(-1, -1, strlen(buf), buf, Scheme::TokenType::T_STRING));
}

Scheme::SchemeObjectPtr Scheme::string_to_fixnum_builtin(SchemeObjectPtr arguments) {
    auto val = std::dynamic_pointer_cast<Scheme::String>(
            std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue();
    return std::make_shared<Scheme::Fixnum>(atol(val->getText().c_str()));
}

Scheme::SchemeObjectPtr Scheme::add_builtin(Scheme::SchemeObjectPtr arguments) {
    long ret = 0;

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            ret += fixnum->getValue();
        }

        arguments = list->getCdr();
    }

    return std::make_shared<Scheme::Fixnum>(ret);
}

Scheme::SchemeObjectPtr Scheme::sub_builtin(Scheme::SchemeObjectPtr arguments) {
    long ret = std::dynamic_pointer_cast<Scheme::Fixnum>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue();
    arguments = std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            ret -= fixnum->getValue();
        }

        arguments = list->getCdr();
    }

    return std::make_shared<Scheme::Fixnum>(ret);
}

Scheme::SchemeObjectPtr Scheme::mul_builtin(Scheme::SchemeObjectPtr arguments) {
    long ret = 1;

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            ret *= fixnum->getValue();
        }

        arguments = list->getCdr();
    }

    return std::make_shared<Scheme::Fixnum>(ret);
}

Scheme::SchemeObjectPtr Scheme::quotient_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto a = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar());
    auto b = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCadr());

    return std::make_shared<Scheme::Fixnum>(a->getValue() / b->getValue());
}

Scheme::SchemeObjectPtr Scheme::remainder_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto a = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar());
    auto b = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCadr());

    return std::make_shared<Scheme::Fixnum>(a->getValue() % b->getValue());
}

Scheme::SchemeObjectPtr Scheme::equal_fixnum_builtin(Scheme::SchemeObjectPtr arguments) {
    long val = std::dynamic_pointer_cast<Scheme::Fixnum>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue();
    arguments = std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            if (val != fixnum->getValue()) {
                return Scheme::Boolean::getFalse();
            }
        }

        arguments = list->getCdr();
    }

    return Scheme::Boolean::getTrue();
}

Scheme::SchemeObjectPtr Scheme::lt_fixnum_builtin(Scheme::SchemeObjectPtr arguments) {
    long val = std::dynamic_pointer_cast<Scheme::Fixnum>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue();
    arguments = std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            long cur = fixnum->getValue();

            if (val < cur) {
                val = cur;
            } else {
                return Scheme::Boolean::getFalse();
            }
        }

        arguments = list->getCdr();
    }

    return Scheme::Boolean::getTrue();
}

Scheme::SchemeObjectPtr Scheme::gt_fixnum_builtin(Scheme::SchemeObjectPtr arguments) {
    long val = std::dynamic_pointer_cast<Scheme::Fixnum>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getValue();
    arguments = std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdr();

    while (not arguments->isEmptyList()) {
        auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

        if (auto fixnum = std::dynamic_pointer_cast<Scheme::Fixnum>(list->getCar())) {
            long cur = fixnum->getValue();

            if (val > cur) {
                val = cur;
            } else {
                return Scheme::Boolean::getFalse();
            }
        }

        arguments = list->getCdr();
    }

    return Scheme::Boolean::getTrue();
}

Scheme::SchemeObjectPtr Scheme::cons_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    return std::make_shared<Scheme::Pair>(list->getCar(), list->getCadr());
}

Scheme::SchemeObjectPtr Scheme::car_builtin(Scheme::SchemeObjectPtr arguments) {
    return std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCaar();
}

Scheme::SchemeObjectPtr Scheme::cdr_builtin(Scheme::SchemeObjectPtr arguments) {
    return std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCdar();
}

Scheme::SchemeObjectPtr Scheme::set_car_builtin(Scheme::SchemeObjectPtr arguments) {
    auto obj = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    std::dynamic_pointer_cast<Scheme::Pair>(obj->getCar())->setCar(obj->getCadr());
    return SchemeObject::ok_symbol;
}

Scheme::SchemeObjectPtr Scheme::set_cdr_builtin(Scheme::SchemeObjectPtr arguments) {
    auto obj = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    std::dynamic_pointer_cast<Scheme::Pair>(obj->getCar())->setCdr(obj->getCadr());
    return SchemeObject::ok_symbol;
}

Scheme::SchemeObjectPtr Scheme::list_builtin(Scheme::SchemeObjectPtr arguments) {
    return arguments;
}

Scheme::SchemeObjectPtr Scheme::eq_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto a = list->getCar();
    auto b = list->getCadr();

    if (a->getType() != b->getType()) {
        return Scheme::Boolean::getFalse();
    }

    switch (a->getType()) {
        case SchemeObject::FIXNUM_TY:
            return std::dynamic_pointer_cast<Scheme::Fixnum>(a)->getValue() ==
                   std::dynamic_pointer_cast<Scheme::Fixnum>(b)->getValue() ?
                    Scheme::Boolean::getTrue() : Scheme::Boolean::getFalse();

        case SchemeObject::CHARACTER_TY:
            return std::dynamic_pointer_cast<Scheme::Character>(a)->getValue() ==
                   std::dynamic_pointer_cast<Scheme::Character>(b)->getValue() ?
                   Scheme::Boolean::getTrue() : Scheme::Boolean::getFalse();

        case SchemeObject::STRING_TY:
            return std::dynamic_pointer_cast<Scheme::String>(a)->getValue()->getText() ==
                   std::dynamic_pointer_cast<Scheme::String>(b)->getValue()->getText() ?
                   Scheme::Boolean::getTrue() : Scheme::Boolean::getFalse();

        default:
            return a == b ? Scheme::Boolean::getTrue() : Scheme::Boolean::getFalse();
    }
}

Scheme::SchemeObjectPtr Scheme::load_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto& filename = std::dynamic_pointer_cast<Scheme::String>(list->getCar())->getValue()->getText();
    auto instream = fopen(filename.c_str(), "r");

    if (instream) {
        Scheme::Reader reader(instream, filename);
        Scheme::SchemeObjectPtr res = nullptr;

        while (not reader.isEof()) {
            auto obj = reader.read();

            if (obj) {
                res = eval(obj, TOPLEVEL_ENV);
            } else {
                //throw ReaderException("error in input");
            }
        }

        return res;
    } else {
        std::string msg{"error reading file: "};
        msg.append(filename);
        throw ReaderException(msg);
    }
}

Scheme::SchemeObjectPtr Scheme::read_builtin(Scheme::SchemeObjectPtr arguments) {
    FILE* instream = arguments->isEmptyList()
                        ? stdin
                        : std::dynamic_pointer_cast<Scheme::InputPort>(
                            std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getInputStream();

    Scheme::Reader reader(instream, "stdin/file");
    auto obj = reader.read();
    return obj == nullptr ? Scheme::SchemeObject::eof_symbol : obj;
}

Scheme::SchemeObjectPtr Scheme::read_char_builtin(Scheme::SchemeObjectPtr arguments) {
    FILE* instream = arguments->isEmptyList()
                     ? stdin
                     : std::dynamic_pointer_cast<Scheme::InputPort>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getInputStream();

    int ch = fgetc(instream);
    return ch == EOF ? Scheme::SchemeObject::eof_symbol : std::make_shared<Scheme::Character>(ch);
}

Scheme::SchemeObjectPtr Scheme::peek_char_builtin(Scheme::SchemeObjectPtr arguments) {
    FILE* instream = arguments->isEmptyList()
                     ? stdin
                     : std::dynamic_pointer_cast<Scheme::InputPort>(
                        std::dynamic_pointer_cast<Scheme::Pair>(arguments)->getCar())->getInputStream();

    int ch = fgetc(instream);
    ungetc(ch, instream);
    return ch == EOF ? Scheme::SchemeObject::eof_symbol : std::make_shared<Scheme::Character>(ch);
}

Scheme::SchemeObjectPtr Scheme::eof_object_builtin(SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

    if (auto arg = std::dynamic_pointer_cast<Scheme::Symbol>(list->getCar())) {
        return arg->getValue()->getText() == "eof" ? Scheme::Boolean::getTrue() : Scheme::Boolean::getFalse();
    } else {
        return Scheme::Boolean::getFalse();
    }
}

Scheme::SchemeObjectPtr Scheme::input_port_builtin(SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    return list->getCar()->isInputPort() ? Scheme::Boolean::getTrue() : Scheme::Boolean::getFalse();
}

Scheme::SchemeObjectPtr Scheme::open_input_port_builtin(SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto filename = std::dynamic_pointer_cast<Scheme::String>(list->getCar())->getValue()->getText();
    FILE* instream = fopen(filename.c_str(), "r");

    if (instream) {
        return std::make_shared<Scheme::InputPort>(instream);
    } else {
        std::string msg{"error opening file: "};
        msg.append(filename);
        throw std::runtime_error(msg);
    }
}

Scheme::SchemeObjectPtr Scheme::close_input_port_builtin(SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto inport = std::dynamic_pointer_cast<Scheme::InputPort>(list->getCar());

    if (inport->closeInputStream() == EOF) {
        throw std::runtime_error("error closing input port");
    } else {
        return Scheme::SchemeObject::ok_symbol;
    }
}

Scheme::SchemeObjectPtr Scheme::write_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    FILE* outstream = list->getCdr()->isEmptyList()
                     ? stdout
                     : std::dynamic_pointer_cast<Scheme::OutputPort>(list->getCadr())->getOutputStream();

    print(list->getCar(), outstream);
    fflush(outstream);
    return Scheme::SchemeObject::ok_symbol;
}

Scheme::SchemeObjectPtr Scheme::write_char_builtin(Scheme::SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    FILE* outstream = list->getCdr()->isEmptyList()
                      ? stdout
                      : std::dynamic_pointer_cast<Scheme::OutputPort>(list->getCadr())->getOutputStream();

    fputc(std::dynamic_pointer_cast<Scheme::Character>(list->getCar())->getValue(), outstream);
    fflush(outstream);
    return Scheme::SchemeObject::ok_symbol;
}

Scheme::SchemeObjectPtr Scheme::output_port_builtin(SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    return list->getCar()->isOutputPort() ? Scheme::Boolean::getTrue() : Scheme::Boolean::getFalse();
}

Scheme::SchemeObjectPtr Scheme::open_output_port_builtin(SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto filename = std::dynamic_pointer_cast<Scheme::String>(list->getCar())->getValue()->getText();
    FILE* outstream = fopen(filename.c_str(), "w");

    if (outstream) {
        return std::make_shared<Scheme::OutputPort>(outstream);
    } else {
        std::string msg{"error opening file: "};
        msg.append(filename);
        throw std::runtime_error(msg);
    }
}

Scheme::SchemeObjectPtr Scheme::close_output_port_builtin(SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);
    auto outport = std::dynamic_pointer_cast<Scheme::OutputPort>(list->getCar());

    if (outport->closeOutputStream() == EOF) {
        throw std::runtime_error("error closing output port");
    } else {
        return Scheme::SchemeObject::ok_symbol;
    }
}

Scheme::SchemeObjectPtr Scheme::error_builtin(SchemeObjectPtr arguments) {
    auto list = std::dynamic_pointer_cast<Scheme::Pair>(arguments);

    while (not list->isEmptyList()) {
        print(list->getCar(), stderr);
        fprintf(stderr, "%s", " ");
        list = std::dynamic_pointer_cast<Scheme::Pair>(list->getCdr());
    }

    throw std::runtime_error("error: exiting");
}
