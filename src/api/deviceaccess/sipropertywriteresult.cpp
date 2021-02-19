#include "sipropertywriteresult.h"

using namespace std;

struct SIPropertyWriteResult::Private_ {
    Private_(SIPropertyID id, SIStatus status): id(id), status(status) {}

    SIPropertyID id;
    SIStatus status;
};

SIPropertyWriteResult::SIPropertyWriteResult() = default;

SIPropertyWriteResult::SIPropertyWriteResult(SIPropertyID id, SIStatus status): private_(make_shared<Private_>(id, status)) {}

SIPropertyID SIPropertyWriteResult::id() const {
    return private_ ? private_->id : 0;
}

void SIPropertyWriteResult::setID(SIPropertyID id) {
    if (private_) {
        detach_();
        private_->id = id;
    }
}

SIStatus SIPropertyWriteResult::status() const {
    return private_ ? private_->status : SIStatus::Error;
}

SIPropertyWriteResult::operator bool() {
    return private_ && private_->status == SIStatus::Success;
}

void SIPropertyWriteResult::detach_() {
    if (private_.use_count() > 1) {
        private_ = make_shared<Private_>(*private_);
    }
}
