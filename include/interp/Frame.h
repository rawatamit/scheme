#ifndef FRAME_H
#define FRAME_H

#include "AST/SchemeObject.h"
#include <map>

namespace Scheme {
class Frame {
public:
    typedef std::map<std::string, Scheme::SchemeObject const*> SymMap;
    typedef SymMap::iterator SymMapIterator;
    typedef SymMap::const_iterator SymMapConstIterator;

private:
    Frame* enclosingframe_;
    SymMap symtab_;
    SymMapIterator lookupIter(const std::string& var);
    SymMapConstIterator lookupConstIter(const std::string& var) const;

public:
    Frame(Frame* enclosingframe);
    virtual ~Frame();
    Frame* getEnclosingFrame();
    Frame const* getEnclosingFrame() const;
    SymMap const& getSymtab() const;
    Scheme::SchemeObject const* lookup(const std::string& obj) const;
    void define(const std::string& var, Scheme::SchemeObject const* val);
    void redefine(const std::string& var, Scheme::SchemeObject const* val);
};
} // namespace Scheme

#endif
