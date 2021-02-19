#include "sipropertyreadresult.h"
#include <utility>

using namespace std;

static const QVariant nullVariant_ = {};

struct SIPropertyReadResult::Private_ {
    Private_(SIPropertyID id, SIStatus status, QVariant value): id(id), status(status), value(move(value)) {}

    SIPropertyID id;
    SIStatus status;
    QVariant value;
};

SIPropertyReadResult::SIPropertyReadResult() = default;

SIPropertyReadResult::SIPropertyReadResult(SIPropertyID id, SIStatus status, const QVariant& value): private_(make_shared<Private_>(id, status, value)) {}

SIPropertyID SIPropertyReadResult::id() const {
    return private_ ? private_->id : 0;
}

void SIPropertyReadResult::setID(SIPropertyID id) {
    if (private_) {
        detach_();
        private_->id = id;
    }
}

SIStatus SIPropertyReadResult::status() const {
    return private_ ? private_->status : SIStatus::Error;
}

const QVariant& SIPropertyReadResult::value() const {
    return private_ ? private_->value : nullVariant_;
}

SIPropertyReadResult::operator bool() const {
    return private_ && private_->status == SIStatus::Success;
}

void SIPropertyReadResult::detach_() {
    if (private_.use_count() > 1) {
        private_ = make_shared<Private_>(*private_);
    }
}
