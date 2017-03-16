#include "interp/Reader.h"
#include "interp/ReaderException.h"
#include "AST/Fixnum.h"
#include "AST/Boolean.h"
#include "AST/Character.h"
#include "AST/String.h"
#include "AST/Pair.h"
#include "AST/Symbol.h"
#include "AST/Quote.h"
#include "AST/Definition.h"
#include "AST/Redefinition.h"
#include "AST/If.h"

Scheme::Reader::Reader(std::istream& in, const std::string& description) :
    ch_(' '), line_(1), column_(0), eof_(false),
    in_(in), description_(description)
{
}

Scheme::Reader::~Reader() {
}

bool Scheme::Reader::isEof() const {
    return eof_;
}

std::string const& Scheme::Reader::getDescription() const {
    return description_;
}

Scheme::SchemeObjectPtr Scheme::Reader::read() {
    int start_line, start_col;
    skipWhitespace(); // start with a valid character

    switch (ch_) {
    case '-':
        return readFixnumOrMinus();
    case '#':
        return readBooleanOrCharacter();
    case '"':
        return readString();
    case '(':
        start_line = line_;
        start_col = column_;
        nextChar(); // skip '('
        return processPair(readPair(start_line, start_col));
    case '\'':
        nextChar(); // skip '
        return std::make_shared<Scheme::Quote>(read());
    case EOF:
        eof_ = true;
        return nullptr;
    }

    if (isdigit(ch_)) {
        return readFixnumOrMinus();
    } else if (isInitial(ch_) or ((ch_ == '+' or ch_ == '-'))) { // FIXME: ch_ CANNOT be '-' here
        return readSymbol();
    }

    nextChar(); // skip this character
    return read();
}

int Scheme::Reader::nextChar() {
    ch_ = in_.get();

    if (ch_ == '\n') {
        ++line_;
        column_ = 0;
    } else {
        ++column_;
    }

    return ch_;
}

void Scheme::Reader::consume(int c) {
    if (c == ch_) {
        nextChar();
    } else {
        throw Scheme::ReaderException("expected 'c'"); // FIXME: fix this
    }
}

void Scheme::Reader::skipWhitespace() {
    while (isspace(ch_))
        nextChar();
}

bool Scheme::Reader::isInitial(int c) const {
    return isalpha(c) || c == '*' || c == '/' || c == '>' ||
           c == '<' || c == '=' || c == '?' || c == '!';
}

bool Scheme::Reader::isDelimiter(int c) const {
    return isspace(c) || c == EOF ||
           c == '('   || c == ')' ||
           c == '"'   || c == ';';
}

Scheme::SchemeObjectPtr Scheme::Reader::readFixnumOrMinus() {
    int line = line_, scol = column_;
    std::string text;

    // do we have a negative number
    if (ch_ == '-') {
        text.push_back('-');
        nextChar();
    }

    // is this the sub procedure?
    if (not isdigit(ch_)) {
        return Scheme::Symbol::getSymbol(std::make_shared<Token>(line, scol, text.size(), text, Scheme::TokenType::T_SYMBOL));
    } else {
        do {
            text.push_back(ch_);
            nextChar();
        } while (isdigit(ch_));

        return std::make_shared<Scheme::Fixnum>(
                std::make_shared<Token>(line, scol, text.size(), text, Scheme::TokenType::T_FIXNUM));
    }
}

Scheme::SchemeObjectPtr Scheme::Reader::readBooleanOrCharacter() {
    int line = line_, scol = column_;
    std::string text;
    text.push_back('#');
    consume('#');

    // boolean
    if (ch_ == 't' or ch_ == 'f') {
        text.push_back(ch_);
        nextChar(); // skip 't' or 'f'
        return Scheme::Boolean::get(
            std::make_shared<Token>(line, scol, text.size(), text, Scheme::TokenType::T_BOOLEAN));
    } else if (ch_ == '\\') { // character
        text.push_back('\\');
        nextChar(); // eat '\'

        switch (ch_) {
        case EOF:
            throw Scheme::ReaderException("eof in character literal");

        // 'space' or 's'
        case 's':
            text.push_back('s');
            nextChar(); // eat 's'
            if (ch_ == 'p') {
                const char* s = "pace";
                for (int i = 0; s[i] != '\0'; ++i) {
                     if (ch_ == s[i]) {
                        text.push_back(ch_);
                        nextChar();
                    } else {
                        throw Scheme::ReaderException("incomplete character literal");
                    }
                }
            }
            return std::make_shared<Scheme::Character>(
                std::make_shared<Token>(line, scol, text.size(), text, Scheme::TokenType::T_CHAR));

        // '\n'
        case '\n':
            text.append("newline");
            return std::make_shared<Scheme::Character>(
                    std::make_shared<Token>(line, scol, 1, text, Scheme::TokenType::T_CHAR));

        // 'newline' or 'n'
        case 'n':
            text.push_back('n');
            nextChar(); // eat 'n'
            if (ch_ == 'e') {
                const char* s = "ewline";
                for (int i = 0; s[i] != '\0'; ++i) {
                    if (ch_ == s[i]) {
                        text.push_back(ch_);
                        nextChar();
                    } else {
                        throw Scheme::ReaderException("incomplete character literal");
                    }
                }
            }
            return std::make_shared<Scheme::Character>(
                std::make_shared<Token>(line, scol, text.size(), text, Scheme::TokenType::T_CHAR));

        default:
            text.push_back(ch_);
            nextChar(); // eat this character
            return std::make_shared<Scheme::Character>(
                std::make_shared<Token>(line, scol, text.size(), text, Scheme::TokenType::T_CHAR));
        }
    }

    throw Scheme::ReaderException("unknown boolean or character literal");
}

Scheme::SchemeObjectPtr Scheme::Reader::readString() {
    int line = line_, scol = column_;
    std::string text;
    nextChar(); // eat '"'

    while (ch_ != '"') {
        switch (ch_) {
        case EOF:
            throw Scheme::ReaderException("eof in string literal");
        case '\\':
            nextChar(); // eat '\'
            if (ch_ == 'n') {
                text.push_back('\n');
            } else {
                text.push_back(ch_);
            }
            break;
        default:
            text.push_back(ch_);
            break;
        }

        nextChar();
    }

    nextChar(); // eat '"'
    return std::make_shared<Scheme::String>(
        std::make_shared<Token>(line, scol, text.size(), text, Scheme::TokenType::T_STRING));
}

Scheme::SchemeObjectPtr Scheme::Reader::processPair(Scheme::SchemeObjectPtr obj) {
    auto pair = std::dynamic_pointer_cast<Scheme::Pair>(obj);

    if (not pair->isEmptyList() and pair->getCar()->isSymbol()) {
        auto sym = std::dynamic_pointer_cast<Scheme::Symbol>(pair->getCar());
        std::string const& name = sym->getValue()->getText();

        if (name == "quote") {
            if (pair->getCdr() == nullptr) {
                throw Scheme::ReaderException("quote form not proper");
            } else if (pair->getCdr()->isEmptyList()) {
                    return pair->getCdr();
            } else {
                auto pcdr = std::dynamic_pointer_cast<Scheme::Pair>(pair->getCdr());
                return std::make_shared<Scheme::Quote>(pcdr->getCar());
            }
        } else if (name == "define") {
            if (auto list = std::dynamic_pointer_cast<Scheme::Pair>(pair->getCdr())) {
                Scheme::SchemeObjectPtr var = list->getCar();
                Scheme::SchemeObjectPtr val = list->getCadr();

                if (var == nullptr or val == nullptr) {
                    throw Scheme::ReaderException("define form not proper");
                } else {
                    return std::make_shared<Scheme::Definition>(var, val);
                }
            } else {
                throw Scheme::ReaderException("define form not proper");
            }
        } else if (name == "set!") {
            if (auto list = std::dynamic_pointer_cast<Scheme::Pair>(pair->getCdr())) {
                Scheme::SchemeObjectPtr var = list->getCar();
                Scheme::SchemeObjectPtr val = list->getCadr();

                if (var == nullptr or val == nullptr) {
                    throw Scheme::ReaderException("set! form not proper");
                } else {
                    return std::make_shared<Scheme::Redefinition>(var, val);
                }
            } else {
                throw Scheme::ReaderException("set! form not proper");
            }
        } else if (name == "if") {
            if (auto list = std::dynamic_pointer_cast<Scheme::Pair>(pair->getCdr())) {
                Scheme::SchemeObjectPtr predicate = list->getCar();
                Scheme::SchemeObjectPtr consequent = list->getCadr();

                if (predicate == nullptr or consequent == nullptr) {
                    throw Scheme::ReaderException("if form not proper");
                } else if (auto alternative = list->getCaddr()) {
                    return std::make_shared<Scheme::If>(predicate, consequent, alternative);
                } else {
                    return std::make_shared<Scheme::If>(predicate, consequent, nullptr);
                }
            } else {
                throw Scheme::ReaderException("if form not proper");
            }
        }
    }

    return pair;
}

// FIXME: line number information might not be correct
Scheme::SchemeObjectPtr Scheme::Reader::readPair(int start_line, int start_col) {
    skipWhitespace();

    if (ch_ == ')') {
        auto empty_list = Scheme::Pair::getEmptyList(start_line, start_col, line_, column_);
        nextChar(); // skip ')'
        return empty_list;
    }

    Scheme::SchemeObjectPtr car = read();
    skipWhitespace();
    
    if (ch_ == '.') { // read improper list
        nextChar(); // eat '.'
        if (isDelimiter(ch_)) {
            Scheme::SchemeObjectPtr cdr = read();
            skipWhitespace();
            if (ch_ == ')') {
                auto pair = std::make_shared<Scheme::Pair>(start_line, start_col, line_, column_, car, cdr);
                nextChar(); // skip ')'
                return pair;
            } else {
                throw Scheme::ReaderException("list not properly terminated");
            }
        } else {
            throw Scheme::ReaderException("dot not followed by delimiter");
        }
    } else { // read proper list
        Scheme::SchemeObjectPtr cdr = readPair(start_line, start_col);
        return std::make_shared<Scheme::Pair>(start_line, start_col, line_, column_, car, cdr);
    }
}

Scheme::SchemeObjectPtr Scheme::Reader::readSymbol() {
    int line = line_, scol = column_;
    std::string text;
    
    while (isInitial(ch_) or isdigit(ch_) or ch_ == '+' or ch_ == '-') {
        text.push_back(ch_);
        nextChar();
    }
    
    if (isDelimiter(ch_)) {
        return Scheme::Symbol::getSymbol(
            std::make_shared<Token>(line, scol, text.size(), text, Scheme::TokenType::T_SYMBOL));
    } else {
        throw Scheme::ReaderException("symbol not followed by delimiter");
    }
}
