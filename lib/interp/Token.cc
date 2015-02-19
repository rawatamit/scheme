#include "interp/Token.h"

Token::Token(int line,
      int column,
      int length,
      std::string const& text,
      Scheme::TokenType type) :
    line_(line), column_(column), length_(length),
    text_(text), type_(type) {
}

Token::~Token() {
}

int Token::getLine() const {
    return line_;
}

int Token::getColumn() const {
    return column_;
}

int Token::getLength() const {
    return length_;
}

std::string const& Token::getText() const {
    return text_;
}

Scheme::TokenType Token::getType() const {
    return type_;
}
