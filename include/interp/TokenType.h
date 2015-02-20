#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

namespace Scheme {
enum TokenType {
    T_FIXNUM, T_BOOLEAN,
    T_CHAR, T_STRING,
    T_SYMBOL,
    T_LPAREN, T_RPAREN,
    T_EOF, T_ERR,
};
}

#endif
