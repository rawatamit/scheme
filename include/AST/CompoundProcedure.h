#ifndef COMPOUND_PROCEDURE_H
#define COMPOUND_PROCEDURE_H

#include "interp/Environment.h"
#include "AST/SchemeObject.h"

namespace Scheme {
    class CompoundProcedure : public SchemeObject {
    private:
        SchemeObjectPtr params_;
        SchemeObjectPtr body_;
        std::shared_ptr<Environment> env_;

    public:
        CompoundProcedure(SchemeObjectPtr params, SchemeObjectPtr body, std::shared_ptr<Environment> env);
        virtual ~CompoundProcedure();
        SchemeObjectPtr getParameters() const;
        SchemeObjectPtr getBody() const;
        std::shared_ptr<Environment> getEnvironment() const;
    };
}

#endif // COMPOUNDPROCEDURE_H
