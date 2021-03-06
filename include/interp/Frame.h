#ifndef FRAME_H
#define FRAME_H

#include "AST/SchemeObject.h"
#include <map>
#include <ostream>

namespace Scheme {
    class Frame {
    public:
        typedef std::map<std::string, Scheme::SchemeObjectPtr> SymMap;
        typedef SymMap::iterator SymMapIterator;
        typedef SymMap::const_iterator SymMapConstIterator;

    private:
        std::shared_ptr<Frame> enclosingframe_;
        SymMap symtab_;
        SymMapIterator lookupIter(const std::string& var);
        SymMapConstIterator lookupConstIter(const std::string& var) const;

    public:
        Frame(std::shared_ptr<Frame> enclosingframe);
        virtual ~Frame();
        std::shared_ptr<Frame> getEnclosingFrame();
        std::shared_ptr<Frame> getEnclosingFrame() const;
        SymMap const& getSymtab() const;
        Scheme::SchemeObjectPtr lookup(const std::string& obj) const;
        void define(const std::string& var, Scheme::SchemeObjectPtr val);
        void redefine(const std::string& var, Scheme::SchemeObjectPtr val);

        //friend std::ostream& operator<<(std::ostream& out, const Frame& frame);
    };
} // namespace Scheme

#endif
