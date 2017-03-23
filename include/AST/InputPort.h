#ifndef INPUT_PORT_H
#define INPUT_PORT_H

#include "AST/SchemeObject.h"
#include <cstdio>

namespace Scheme {
    class InputPort : public SchemeObject {
    private:
        FILE* in_;

    public:
        InputPort(FILE* in);
        virtual ~InputPort();
        FILE* getInputStream() const;
        int closeInputStream();
    };
}

#endif // INPUT_PORT_H
