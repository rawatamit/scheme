#include "interp/Reader.h"
#include "interp/ReaderException.h"
#include "AST/Fixnum.h"
#include "AST/Boolean.h"
#include "AST/Character.h"
#include "AST/String.h"
#include "AST/Pair.h"
#include "AST/Symbol.h"

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

Scheme::SchemeObject* Scheme::Reader::read() {
    int start_line, start_col;
    skipWhitespace(); // start with a valid character

    switch (ch_) {
    case '-': // FIXME: be more precise
        return readFixnumOrMinus();
    case '#':
        return readBooleanOrCharacter();
    case '"':
        return readString();
    case '(':
        start_line = line_;
        start_col = column_;
        readChar(); // skip '('
        return readPair(start_line, start_col);
    case EOF:
        eof_ = true;
        return nullptr;
    }

    if (isdigit(ch_)) {
        return readFixnumOrMinus();
    } else if (isInitial(ch_) or ((ch_ == '+' or ch_ == '-'))) { // FIXME: be more precise
        return readSymbol();
    }

    readChar(); // skip this character
    return read();
}

int Scheme::Reader::readChar() {
    ch_ = in_.get();

    if (ch_ == '\n') {
        ++line_;
        column_ = 0;
    } else {
        ++column_;
    }

    return ch_;
}

void Scheme::Reader::skipWhitespace() {
    while (isspace(ch_))
        readChar();
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

Scheme::SchemeObject* Scheme::Reader::readFixnumOrMinus() {
    int line = line_, scol = column_;
    std::string text;

    // do we have a negative number
    if (ch_ == '-') {
        text.push_back('-');
        readChar();
    }

    do {
        text.push_back(ch_);
        readChar();
    } while (isdigit(ch_));

    return new Scheme::Fixnum(
            new Token(line, scol, text.size(), text, Scheme::TokenType::T_FIXNUM));
}

Scheme::SchemeObject* Scheme::Reader::readBooleanOrCharacter() {
    int line = line_, scol = column_;
    std::string text;
    text.push_back('#');
    readChar(); // eat '#'

    // boolean
    if (ch_ == 't' or ch_ == 'f') {
        text.push_back(ch_);
        readChar(); // skip 't' or 'f'
        return Scheme::Boolean::get(
            new Token(line, scol, text.size(), text, Scheme::TokenType::T_BOOLEAN));
    } else if (ch_ == '\\') { // character
        text.push_back('\\');
        readChar(); // eat '\'
        switch (ch_) {
        case EOF:
            throw new Scheme::ReaderException("eof in character literal");

        // 'space' or 's'
        case 's':
            text.push_back('s');
            readChar(); // eat 's'
            if (ch_ == 'p') {
                const char* s = "pace";
                for (int i = 0; s[i] != '\0'; ++i) {
                     if (ch_ == s[i]) {
                        text.push_back(ch_);
                        readChar();
                    } else {
                        throw new Scheme::ReaderException("incomplete character literal");
                    }
                }
            }
            return new Scheme::Character(
                new Token(line, scol, text.size(), text, Scheme::TokenType::T_CHAR));

        // 'newline' or 'n'
        case 'n':
            text.push_back('n');
            readChar(); // eat 'n'
            if (ch_ == 'e') {
                const char* s = "ewline";
                for (int i = 0; s[i] != '\0'; ++i) {
                    if (ch_ == s[i]) {
                        text.push_back(ch_);
                        readChar();
                    } else {
                        throw new Scheme::ReaderException("incomplete character literal");
                    }
                }
            }
            return new Scheme::Character(
                new Token(line, scol, text.size(), text, Scheme::TokenType::T_CHAR));

        default:
            text.push_back(ch_);
            readChar(); // eat this character
            return new Scheme::Character(
                new Token(line, scol, text.size(), text, Scheme::TokenType::T_CHAR));
        }
    }

    throw new Scheme::ReaderException("unknown boolean or character literal");
}

Scheme::SchemeObject* Scheme::Reader::readString() {
    int line = line_, scol = column_;
    std::string text;
    readChar(); // eat '"'

    while (ch_ != '"') {
        switch (ch_) {
        case EOF:
            throw new Scheme::ReaderException("eof in string literal");
        case '\\':
            readChar(); // eat '\'
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

        readChar();
    }

    readChar(); // eat '"'
    return new Scheme::String(
        new Token(line, scol, text.size(), text, Scheme::TokenType::T_STRING));
}

// FIXME: line number information might not be correct
Scheme::SchemeObject* Scheme::Reader::readPair(int start_line, int start_col) {
    skipWhitespace();

    if (ch_ == ')') {
        Scheme::Pair* empty_list =
            Scheme::Pair::getEmptyList(start_line, start_col, line_, column_);
        readChar(); // skip ')'
        return empty_list;
    }

    Scheme::SchemeObject* car = read();
    skipWhitespace();
    if (ch_ == '.') { // read improper list
        readChar(); // eat '.'
        if (isDelimiter(ch_)) {
            Scheme::SchemeObject* cdr = read();
            skipWhitespace();
            if (ch_ == ')') {
                Scheme::Pair* pair =
                    new Scheme::Pair(start_line, start_col, line_, column_, car, cdr);
                readChar(); // skip ')'
                return pair;
            } else {
                throw Scheme::ReaderException("list not properly terminated");
            }
        } else {
            throw Scheme::ReaderException("dot not followed by delimiter");
        }
    } else { // read proper list
        Scheme::SchemeObject* cdr = readPair(start_line, start_col);
        return new Scheme::Pair(start_line, start_col, line_, column_, car, cdr);
    }
}

Scheme::SchemeObject* Scheme::Reader::readSymbol() {
    int line = line_, scol = column_;
    std::string text;
    
    while (isInitial(ch_) or isdigit(ch_) or ch_ == '+' or ch_ == '-') {
        text.push_back(ch_);
        readChar();
    }
    
    if (isDelimiter(ch_)) {
        return Scheme::Symbol::getSymbol(
            new Token(line, scol, text.size(), text, Scheme::TokenType::T_SYMBOL));
    } else {
        throw Scheme::ReaderException("symbol not followed by delimiter");
    }
}
