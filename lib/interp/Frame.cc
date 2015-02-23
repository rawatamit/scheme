#include "interp/Frame.h"
#include "interp/EnvironmentException.h"

Scheme::Frame::Frame(Scheme::Frame* enclosingframe) :
    enclosingframe_(enclosingframe)
{
}

Scheme::Frame::~Frame() {
    enclosingframe_ = nullptr;
    symtab_.clear();
}

Scheme::Frame const* Scheme::Frame::getEnclosingFrame() const {
    return enclosingframe_;
}

EnvMap::iterator Scheme::Frame::lookup(Scheme::SchemeObject* obj) {
    Frame* frame = this;
    while (frame != nullptr) {
        auto it = symtab_.find(obj);
        if (it != symtab_.end()) {
            return it->second;
        } else {
            frame = frame->getEnclosingFrame();
        }
    }
    return symtab_.end();
}

Scheme::SchemeObject* Scheme::Frame::lookup(Scheme::SchemeObject* obj) const {
    EnvMap::iterator it = lookup(obj);
    return it == symtab_.end() ? nullptr : it->second;
}

void Scheme::Frame::define(Scheme::SchemeObject* obj, Scheme::SchemeObject* val) {
    symtab_[obj] = val;
}

void Scheme::Frame::redefine(Scheme::SchemeObject* obj, Scheme::SchemeObject* val) {
    EnvMap::iterator it = lookup(obj);
    if (it != symtab_.end()) {
        it->second = val;
    } else {
        throw new EnvironmentException("unbound variable");
    }
}
