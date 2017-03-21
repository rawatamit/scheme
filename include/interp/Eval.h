#ifndef EVAL_H
#define EVAL_H

#include "AST/SchemeObject.h"
#include "interp/Environment.h"

namespace Scheme {
    Scheme::SchemeObjectPtr eval(Scheme::SchemeObjectPtr obj,
                                 Scheme::EnvironmentPtr env);

    Scheme::SchemeObjectPtr listOfValues(Scheme::SchemeObjectPtr obj,
                                         Scheme::EnvironmentPtr env);
} // namespace Scheme

#endif
