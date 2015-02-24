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

Scheme::Frame* Scheme::Frame::getEnclosingFrame() {
    return enclosingframe_;
}

Scheme::Frame const* Scheme::Frame::getEnclosingFrame() const {
    return enclosingframe_;
}

Scheme::Frame::SymMap const& Scheme::Frame::getSymtab() const {
    return symtab_;
}

// FIXME: these two functions are same, code duplication!
Scheme::Frame::SymMapIterator Scheme::Frame::lookupIter(Scheme::SchemeObject const* obj) {
    Frame* frame = this;
    while (frame != nullptr) {
        SymMapIterator it = frame->symtab_.find(obj);
        if (it != frame->symtab_.end()) {
            return it;
        } else {
            frame = frame->getEnclosingFrame();
        }
    }
    return symtab_.end();
}

Scheme::Frame::SymMapConstIterator Scheme::Frame::lookupConstIter(Scheme::SchemeObject const* obj) const {
    Frame const* frame = this;
    while (frame != nullptr) {
        SymMapConstIterator it = frame->symtab_.find(obj);
        if (it != frame->symtab_.end()) {
            return it;
        } else {
            frame = frame->getEnclosingFrame();
        }
    }
    return symtab_.end();
}

Scheme::SchemeObject const* Scheme::Frame::lookup(Scheme::SchemeObject const* obj) const {
    SymMapConstIterator it = lookupConstIter(obj);
    return it == symtab_.end() ? nullptr : it->second;
}

void Scheme::Frame::define(Scheme::SchemeObject const* obj, Scheme::SchemeObject const* val) {
    symtab_[obj] = val;
}

void Scheme::Frame::redefine(Scheme::SchemeObject const* obj, Scheme::SchemeObject const* val) {
    SymMapIterator it = lookupIter(obj);
    if (it != symtab_.end()) {
        it->second = val;
    } else {
        throw new EnvironmentException("unbound variable");
    }
}
