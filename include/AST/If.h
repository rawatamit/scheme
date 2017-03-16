#ifndef IF_H
#define IF_H

#include "AST/SchemeObject.h"

namespace Scheme {
    class If : public SchemeObject {
    private:
        SchemeObjectPtr predicate_;
        SchemeObjectPtr consequent_;
        SchemeObjectPtr alternative_;

    public:
        If(SchemeObjectPtr predicate, SchemeObjectPtr consequent, SchemeObjectPtr alternative);
        virtual ~If();

        SchemeObjectPtr getPredicate() const;
        SchemeObjectPtr getConsequent() const;
        SchemeObjectPtr getAlternative() const;
    };
} // namespace Scheme

#endif // IF_H
