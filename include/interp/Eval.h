#ifndef EVAL_H
#define EVAL_H

#include "AST/SchemeObject.h"
#include "interp/Environment.h"

namespace Scheme {
Scheme::SchemeObject const* eval(Scheme::SchemeObject const* obj,
                                 Scheme::Environment* env);
} // namespace Scheme

#endif
