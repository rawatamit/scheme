#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "interp/Frame.h"

namespace Scheme {
class Environment {
private:
    Frame* globalframe_;
    Frame* currentframe_;
public:
    Environment();
    virtual ~Environment();
    Frame* getGlobalFrame();
    Frame const* getGlobalFrame() const;
    Frame* getCurrentFrame();
    Frame const* getCurrentFrame() const;
    void beginFrame();
    void endFrame();
};
} // namespace Scheme

#endif
