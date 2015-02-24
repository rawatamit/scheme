#ifndef FRAME_H
#define FRAME_H

#include "AST/SchemeObject.h"
#include <map>

namespace Scheme {
class Frame {
public:
    typedef std::map<Scheme::SchemeObject const*, Scheme::SchemeObject const*> SymMap;
    typedef SymMap::iterator SymMapIterator;
    typedef SymMap::const_iterator SymMapConstIterator;
private:
    Frame* enclosingframe_;
    SymMap symtab_;
    SymMapIterator lookupIter(Scheme::SchemeObject const* obj);
    SymMapConstIterator lookupConstIter(Scheme::SchemeObject const* obj) const;
public:
    Frame(Frame* enclosingframe);
    virtual ~Frame();
    Frame* getEnclosingFrame();
    Frame const* getEnclosingFrame() const;
    SymMap const& getSymtab() const;
    Scheme::SchemeObject const* lookup(Scheme::SchemeObject const* obj) const;
    void define(Scheme::SchemeObject const* obj, Scheme::SchemeObject const* val);
    void redefine(Scheme::SchemeObject const* obj, Scheme::SchemeObject const* val);
};
} // namespace Scheme

#endif
