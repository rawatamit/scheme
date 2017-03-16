#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "interp/Frame.h"
#include <memory>

namespace Scheme {
class Environment {
private:
    std::shared_ptr<Scheme::Frame> globalframe_;
    std::shared_ptr<Scheme::Frame> currentframe_;

public:
    Environment();
    virtual ~Environment();
    std::shared_ptr<Scheme::Frame> getGlobalFrame();
    std::shared_ptr<Scheme::Frame> getGlobalFrame() const;
    std::shared_ptr<Scheme::Frame> getCurrentFrame();
    std::shared_ptr<Scheme::Frame> getCurrentFrame() const;
    void beginFrame();
    void endFrame();
};
} // namespace Scheme

#endif
