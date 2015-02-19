#include "interp/ReaderException.h"

Scheme::ReaderException::ReaderException(const char* msg) :
    std::runtime_error(msg)
{
}


Scheme::ReaderException::ReaderException(const std::string& msg) :
    std::runtime_error(msg)
{
}

Scheme::ReaderException::~ReaderException() {
}
