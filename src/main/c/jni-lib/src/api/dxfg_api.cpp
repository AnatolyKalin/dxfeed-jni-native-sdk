// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNICommon.hpp"
#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/DxSubscription.hpp"

dxfg_endpoint_state_change_listener_t* dxfg_PropertyChangeListener_new(graal_isolatethread_t* thread,
                                                                       dxfg_endpoint_state_change_listener_func userFunc,
                                                                       void* userData)
{
  auto stateChangeListener = new dxfeed::DxStateChangeListener(thread, userFunc, userData);
  return reinterpret_cast<dxfg_endpoint_state_change_listener_t*>(stateChangeListener);
}

dxfg_feed_event_listener_t* dxfg_DXFeedEventListener_new(graal_isolatethread_t* thread,
                                                         dxfg_feed_event_listener_function user_func, void* user_data)
{
  return reinterpret_cast<dxfg_feed_event_listener_t*>(new dxfeed::DxEventListener(thread, user_func, user_data));
}

int dxfg_JavaObjectHandler_release(graal_isolatethread_t* thread, dxfg_java_object_handler* object) {
  std::cout << "dxfg_JavaObjectHandler_release: ";
  if (object) {
    jobject pObject = object->dxfg_java_object_handle;
    std::cout << "\tdxfg_java_object_handle: " << std::hex << pObject;
    const auto& name = pObject
        ? dxfeed::jni::internal::javaLangClass->getName(thread, pObject) : "";
    if (!name.empty()) {
      std::cout << ", name: " << name;
      if (name == dxfeed::DxEndpoint::JAVA_CLASS_NAME) {
        dxfg_DXEndpoint_release(thread, reinterpret_cast<dxfg_endpoint_t*>(object));
      } else if (name == dxfeed::DxEndpointBuilder::JAVA_CLASS_NAME) {
        dxfg_DXEndpointBuilder_release(thread, reinterpret_cast<dxfg_endpoint_builder_t*>(object));
      } else if (name == dxfeed::DxSubscription::JAVA_CLASS_NAME) {
        dxfg_DXSubscription_release(thread, reinterpret_cast<dxfg_subscription_t*> (object));
      } else if (name == dxfeed::DxStateChangeListener::JAVA_CLASS_NAME) {
        auto* pStateChangeListener = reinterpret_cast<dxfeed::DxStateChangeListener*>(object);
        // can be called from java later, so will be suicided automatically
        pStateChangeListener->closeFromNative();
      } else if (name == dxfeed::DxEventListener::JAVA_CLASS_NAME) {
        auto* pEventListener = reinterpret_cast<dxfeed::DxEventListener*>(object);
        // can be called from java later, so will be suicided automatically
        pEventListener->closeFromNative();
      } else {
        std::cerr << ", LEAKED: " << std::hex << pObject;
      }
      std::cout << std::endl;
    } else {
      std::cerr << ", already released: " << std::hex << pObject << std::endl;
    }
    thread->DeleteLocalRef(pObject);
  }
  return JNI_OK; // todo: think
}



