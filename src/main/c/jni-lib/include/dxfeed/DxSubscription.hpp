// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_

#include <jni.h>
#include <string>
#include <vector>

#include "api/dxfg_events.h"
#include "api/dxfg_subscription.h"

namespace dxfeed {
  struct DxSubscription final {
    DxSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType);
    ~DxSubscription();

    DxSubscription(const DxSubscription& other) = delete;
    DxSubscription(DxSubscription&& other) = delete;
    DxSubscription& operator=(const DxSubscription& other) = delete;
    DxSubscription& operator=(DxSubscription&& other) = delete;

    void addListener(dxfg_feed_event_listener_t* listener) const;
    void addSymbol(const std::string& symbol) const;
    void setSymbol(const std::string& symbol) const;
    void close() const;

  private:
    jobject subscription_;
    JNIEnv* env_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_