#include "interp/EnvironmentException.h"

Scheme::EnvironmentException::EnvironmentException(const char* msg) :
    runtime_error(msg)
{
}

Scheme::EnvironmentException::EnvironmentException(const std::string& msg) :
    runtime_error(msg)
{
}

Scheme::EnvironmentException::~EnvironmentException() {
}
