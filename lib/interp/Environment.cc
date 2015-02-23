#include "interp/Environment.h"

Scheme::Environment::Environment() {
    globalframe_ = new Frame(nullptr);
    currentframe_ = globalframe_;
}

Scheme::Environment::~Environment() {
    currentframe_ = nullptr;
    delete globalframe_;
}

Scheme::Frame* Scheme::Environment::getCurrentFrame() {
    return currentframe_;
}

const Scheme::Frame* Scheme::Environment::getCurrentFrame() const {
    return currentframe_;
}

Scheme::Frame* Scheme::Environment::getGlobalFrame() {
    return globalframe_;
}

const Scheme::Frame* Scheme::Environment::getGlobalFrame() const {
    return globalframe_;
}

void Scheme::Environment::beginFrame() {
    currentframe_ = new Frame(currentframe_);
}

void Scheme::Environment::endFrame() {
    currentframe_ = currentframe_->getEnclosingFrame();
}
