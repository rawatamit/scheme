#ifndef SCHEME_ENVIRONMENT_H
#define SCHEME_ENVIRONMENT_H

#include "AST/SchemeObject.h"
#include "interp/Environment.h"

// encapsulates the environment as a scheme object
// needed to implement eval
namespace Scheme {
    class SchemeEnvironment : public SchemeObject {
    private:
        EnvironmentPtr env_;

    public:
        SchemeEnvironment() : SchemeObject(SchemeObject::ENV_TY), env_(std::make_shared<Environment>()) {}
        SchemeEnvironment(EnvironmentPtr env) : SchemeObject(SchemeObject::ENV_TY), env_(env) {}
        virtual ~SchemeEnvironment() {}
        EnvironmentPtr getEnvironment() { return env_; }
    };
}

#endif // SCHEME_ENVIRONMENT_H
