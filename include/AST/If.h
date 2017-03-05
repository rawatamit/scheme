#ifndef IF_H
#define IF_H

#include "AST/SchemeObject.h"

namespace Scheme {
    class If : public SchemeObject {
    private:
        SchemeObject const* predicate_;
        SchemeObject const* consequent_;
        SchemeObject const* alternative_;

    public:
        If(SchemeObject const* predicate, SchemeObject const* consequent, SchemeObject const* alternative);
        virtual ~If();

        SchemeObject const* getPredicate() const;
        SchemeObject const* getConsequent() const;
        SchemeObject const* getAlternative() const;
    };
} // namespace Scheme

#endif // IF_H
