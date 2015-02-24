#ifndef EVAL_EXCEPTION_H
#define EVAL_EXCEPTION_H

#include <stdexcept>

namespace Scheme {
class EvalException : public std::runtime_error {
public:
    EvalException(const char* msg);
    EvalException(std::string const& msg);
    virtual ~EvalException() throw();
};
} // namespace Scheme

#endif
