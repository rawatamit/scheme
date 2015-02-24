#include "interp/EvalException.h"

Scheme::EvalException::EvalException(const char* msg) :
    std::runtime_error(msg)
{
}


Scheme::EvalException::EvalException(const std::string& msg) :
    std::runtime_error(msg)
{
}

Scheme::EvalException::~EvalException() {
}
