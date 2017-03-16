#ifndef PRINT_H
#define PRINT_H

#include "AST/SchemeObject.h"
#include <ostream>

namespace Scheme {
void print(Scheme::SchemeObjectPtr obj, std::ostream& out);
} // namespace Scheme

#endif
