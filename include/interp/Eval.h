#ifndef EVAL_H
#define EVAL_H

#include "AST/SchemeObject.h"
#include "interp/Environment.h"

namespace Scheme {
    Scheme::SchemeObjectPtr eval(Scheme::SchemeObjectPtr obj,
                                 std::shared_ptr<Scheme::Environment> env);

    Scheme::SchemeObjectPtr listOfValues(Scheme::SchemeObjectPtr obj,
                                         std::shared_ptr<Scheme::Environment> env);
} // namespace Scheme

#endif
