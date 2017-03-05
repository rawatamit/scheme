#include "interp/Token.h"

Scheme::Token::Token(int line,
      int column,
      int length,
      std::string const& text,
      Scheme::TokenType type) :
    line_(line), column_(column), length_(length),
    text_(text), type_(type) {
}

Scheme::Token::~Token() {
}

int Scheme::Token::getLine() const {
    return line_;
}

int Scheme::Token::getColumn() const {
    return column_;
}

int Scheme::Token::getLength() const {
    return length_;
}

std::string const& Scheme::Token::getText() const {
    return text_;
}

Scheme::TokenType Scheme::Token::getType() const {
    return type_;
}
