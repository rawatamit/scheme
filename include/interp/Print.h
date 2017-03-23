#ifndef PRINT_H
#define PRINT_H

#include "AST/SchemeObject.h"
#include <cstdio>

namespace Scheme {
void print(Scheme::SchemeObjectPtr obj, FILE* out);
} // namespace Scheme

#endif
