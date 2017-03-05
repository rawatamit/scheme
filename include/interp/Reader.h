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
    Scheme::SchemeObject const* readFixnumOrMinus();
    Scheme::SchemeObject const* readBooleanOrCharacter();
    Scheme::SchemeObject const* readString();
    Scheme::SchemeObject const* readPair(int start_line, int start_col);
    Scheme::SchemeObject const* processPair(Scheme::SchemeObject const* obj);
    Scheme::SchemeObject const* readSymbol();
public:
    Reader(std::istream& in_, std::string const& description);
    virtual ~Reader();
    bool isEof() const;
    Scheme::SchemeObject const* read();
    std::string const& getDescription() const;
};
} // namespace Scheme

#endif
