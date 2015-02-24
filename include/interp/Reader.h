#ifndef READER_H
#define READER_H

#include "interp/Token.h"
#include "AST/SchemeObject.h"
#include <string>
#include <istream>

namespace Scheme {
class Reader {
private:
    int ch_;
    int line_;
    int column_;
    bool eof_;
    std::istream& in_;
    std::string const& description_;

    int nextChar();
    void consume(int c);
    void skipWhitespace();
    bool isInitial(int ch) const;
    bool isDelimiter(int ch) const;
    Scheme::SchemeObject* readFixnumOrMinus();
    Scheme::SchemeObject* readBooleanOrCharacter();
    Scheme::SchemeObject* readString();
    Scheme::SchemeObject* readPair(int start_line, int start_col);
    Scheme::SchemeObject* processPair(Scheme::SchemeObject* obj);
    Scheme::SchemeObject* readSymbol();
public:
    Reader(std::istream& in_, std::string const& description);
    virtual ~Reader();
    bool isEof() const;
    Scheme::SchemeObject* read();
    std::string const& getDescription() const;
};
} // namespace Scheme

#endif
