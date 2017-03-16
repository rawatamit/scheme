#include "interp/Frame.h"
#include "interp/EnvironmentException.h"

Scheme::Frame::Frame(std::shared_ptr<Scheme::Frame> enclosingframe) :
    enclosingframe_(enclosingframe)
{
}

Scheme::Frame::~Frame() {
    symtab_.clear();
}

std::shared_ptr<Scheme::Frame> Scheme::Frame::getEnclosingFrame() {
    return enclosingframe_;
}

std::shared_ptr<Scheme::Frame> Scheme::Frame::getEnclosingFrame() const {
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
            frame = frame->getEnclosingFrame().get();
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
            frame = frame->getEnclosingFrame().get();
        }
    }

    return symtab_.end();
}

Scheme::SchemeObjectPtr Scheme::Frame::lookup(const std::string& var) const {
    SymMapConstIterator it = lookupConstIter(var);
    return it == symtab_.end() ? nullptr : it->second;
}

void Scheme::Frame::define(const std::string& var, Scheme::SchemeObjectPtr val) {
    symtab_[var] = val;
}

void Scheme::Frame::redefine(const std::string& var, Scheme::SchemeObjectPtr val) {
    SymMapIterator it = lookupIter(var);

    if (it != symtab_.end()) {
        it->second = val;
    } else {
        throw EnvironmentException("unbound variable");
    }
}
