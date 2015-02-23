#ifndef FRAME_H
#define FRAME_H

#include "AST/SchemeObject.h"
#include <map>

namespace Scheme {
class Frame {
public:
    typedef std::map<Scheme::SchemeObject*, Scheme::SchemeObject*> EnvMap;
private:
    Frame* enclosingframe_;
    EnvMap symtab_;
    EnvMap::iterator lookup(Scheme::SchemeObject* obj); // lookup var
public:
    Frame(Frame* enclosingframe);
    virtual ~Frame();
    Frame const* getEnclosingFrame() const;
    Scheme::SchemeObject* lookup(Scheme::SchemeObject* obj) const;
    void define(Scheme::SchemeObject* obj, Scheme::SchemeObject* val);
    void redefine(Scheme::SchemeObject* obj, Scheme::SchemeObject* val);
};
} // namespace Scheme

#endif
