#ifndef READER_EXCEPTION_H
#define READER_EXCEPTION_H

#include <stdexcept>

namespace Scheme {
class ReaderException : public std::runtime_error {
public:
    ReaderException(const char* msg);
    ReaderException(std::string const& msg);
    virtual ~ReaderException() throw();
};
} // namespace Scheme

#endif
