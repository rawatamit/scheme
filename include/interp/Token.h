#ifndef TOKEN_H
#define TOKEN_H

#include "interp/TokenType.h"
#include <string>

namespace Scheme {
    class Token {
    private:
        int line_;
        int column_;
        int length_;
        std::string text_;
        Scheme::TokenType type_;

    public:
        Token(int line,
              int column,
              int length,
              std::string const &text,
              Scheme::TokenType type);

        virtual ~Token();

        int getLine() const;

        int getColumn() const;

        int getLength() const;

        std::string const &getText() const;

        Scheme::TokenType getType() const;
    };
}

#endif
