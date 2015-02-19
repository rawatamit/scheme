#ifndef EVAL_H
#define EVAL_H

#include "AST/SchemeObject.h"

namespace Scheme {
Scheme::SchemeObject* eval(Scheme::SchemeObject* obj);
} // namespace Scheme

#endif
