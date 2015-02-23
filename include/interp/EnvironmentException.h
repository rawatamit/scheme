#ifndef ENVIRONMENT_EXCEPTION_H
#define ENVIRONMENT_EXCEPTION_H

#include <stdexcept>

namespace Scheme {
class EnvironmentException : public std::runtime_error {
public:
    EnvironmentException(const char* msg);
    EnvironmentException(std::string const& msg);
    virtual ~EnvironmentException() throw();
};
} // namespace Scheme

#endif
