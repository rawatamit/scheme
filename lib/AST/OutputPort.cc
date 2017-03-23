#include "AST/OutputPort.h"

Scheme::OutputPort::OutputPort(FILE* out) :
        Scheme::SchemeObject(Scheme::SchemeObject::OUTPUT_PORT_TY), out_(out)
{}

Scheme::OutputPort::~OutputPort()
{}

FILE* Scheme::OutputPort::getOutputStream() const {
    return out_;
}

int Scheme::OutputPort::closeOutputStream() {
    return fclose(out_);
}
