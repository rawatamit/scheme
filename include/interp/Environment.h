#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "interp/Frame.h"
#include <memory>
#include <ostream>

namespace Scheme {
    class Environment {
    private:
        std::shared_ptr<Scheme::Frame> frame_;

    public:
        Environment();
        Environment(std::shared_ptr<Environment> env);
        virtual ~Environment();
        std::shared_ptr<Scheme::Frame> getCurrentFrame();
        std::shared_ptr<Scheme::Frame> getCurrentFrame() const;
        void beginFrame();
        void endFrame();

        //friend std::ostream& operator<<(std::ostream& out, const Environment& env);
    };

    typedef std::shared_ptr<Environment> EnvironmentPtr;
} // namespace Scheme

#endif
