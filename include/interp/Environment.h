#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "interp/Frame.h"

namespace Scheme {
class Environment {
private:
    Scheme::Frame* globalframe_;
    Scheme::Frame* currentframe_;
public:
    Environment();
    virtual ~Environment();
    Scheme::Frame* getGlobalFrame();
    Scheme::Frame const* getGlobalFrame() const;
    Scheme::Frame* getCurrentFrame();
    Scheme::Frame const* getCurrentFrame() const;
    void beginFrame();
    void endFrame();
};
} // namespace Scheme

#endif
