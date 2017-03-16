#include "interp/Environment.h"

Scheme::Environment::Environment() {
    globalframe_ = std::make_shared<Frame>(nullptr);
    currentframe_ = globalframe_;
}

Scheme::Environment::~Environment() {
}

std::shared_ptr<Scheme::Frame> Scheme::Environment::getCurrentFrame() {
    return currentframe_;
}

std::shared_ptr<Scheme::Frame> Scheme::Environment::getCurrentFrame() const {
    return currentframe_;
}

std::shared_ptr<Scheme::Frame> Scheme::Environment::getGlobalFrame() {
    return globalframe_;
}

std::shared_ptr<Scheme::Frame> Scheme::Environment::getGlobalFrame() const {
    return globalframe_;
}

void Scheme::Environment::beginFrame() {
    currentframe_ = std::make_shared<Frame>(currentframe_);
}

void Scheme::Environment::endFrame() {
    currentframe_ = currentframe_->getEnclosingFrame();
}
