#include "AST/InputPort.h"

Scheme::InputPort::InputPort(FILE* in) :
    Scheme::SchemeObject(Scheme::SchemeObject::INPUT_PORT_TY), in_(in)
{}

Scheme::InputPort::~InputPort()
{}

FILE* Scheme::InputPort::getInputStream() const {
    return in_;
}

int Scheme::InputPort::closeInputStream() {
    return fclose(in_);
}
