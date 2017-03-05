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
Scheme::Frame::SymMapIterator Scheme::Frame::lookupIter(const std::string& var) {
    Frame* frame = this;

    while (frame != nullptr) {
        SymMapIterator it = frame->symtab_.find(var);

        if (it != frame->symtab_.end()) {
            return it;
        } else {
            frame = frame->getEnclosingFrame();
        }
    }

    return symtab_.end();
}

Scheme::Frame::SymMapConstIterator Scheme::Frame::lookupConstIter(const std::string& var) const {
    Frame const* frame = this;

    while (frame != nullptr) {
        SymMapConstIterator it = frame->symtab_.find(var);

        if (it != frame->symtab_.end()) {
            return it;
        } else {
            frame = frame->getEnclosingFrame();
        }
    }

    return symtab_.end();
}

Scheme::SchemeObject const* Scheme::Frame::lookup(const std::string& var) const {
    SymMapConstIterator it = lookupConstIter(var);
    return it == symtab_.end() ? nullptr : it->second;
}

void Scheme::Frame::define(const std::string& var, Scheme::SchemeObject const* val) {
    symtab_[var] = val;
}

void Scheme::Frame::redefine(const std::string& var, Scheme::SchemeObject const* val) {
    SymMapIterator it = lookupIter(var);

    if (it != symtab_.end()) {
        it->second = val;
    } else {
        throw new EnvironmentException("unbound variable");
    }
}
