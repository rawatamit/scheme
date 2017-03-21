#include "interp/Environment.h"

Scheme::Environment::Environment() {
    frame_ = std::make_shared<Frame>(nullptr);
}

Scheme::Environment::Environment(EnvironmentPtr env) {
    frame_ = std::make_shared<Frame>(env->getCurrentFrame());
}

Scheme::Environment::~Environment() {
}

std::shared_ptr<Scheme::Frame> Scheme::Environment::getCurrentFrame() {
    return frame_;
}

std::shared_ptr<Scheme::Frame> Scheme::Environment::getCurrentFrame() const {
    return frame_;
}

void Scheme::Environment::beginFrame() {
    frame_ = std::make_shared<Frame>(frame_);
}

void Scheme::Environment::endFrame() {
    frame_ = frame_->getEnclosingFrame();
}

namespace Scheme {
    std::ostream& operator<<(std::ostream &out, const Scheme::Environment &env) {
        unsigned long tab = 0;

        for (auto frame = env.getCurrentFrame(); frame != nullptr; frame = frame->getEnclosingFrame()) {
            out << std::string(" ", tab) << *frame << '\n';
            tab += 4;
        }

        return out;
    }
}
