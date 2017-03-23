#ifndef OUTPUT_PORT_H
#define OUTPUT_PORT_H

#include "AST/SchemeObject.h"
#include <cstdio>

namespace Scheme {
    class OutputPort : public SchemeObject {
    private:
        FILE* out_;

    public:
        OutputPort(FILE* out);
        virtual ~OutputPort();
        FILE* getOutputStream() const;
        int closeOutputStream();
    };
}

#endif // OUTPUT_PORT_H
