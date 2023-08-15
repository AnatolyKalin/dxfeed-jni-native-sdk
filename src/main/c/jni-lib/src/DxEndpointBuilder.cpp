// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed {
  using namespace jni;
  
  DxEndpointBuilder::DxEndpointBuilder(JNIEnv* env) {
    jobject jDxEndpointBuilder = DxEndpoint::newBuilder(env);
    dxEndpointBuilder_ = env->NewGlobalRef(jDxEndpointBuilder);
    env->DeleteLocalRef(jDxEndpointBuilder);
  }

  DxEndpointBuilder::~DxEndpointBuilder() {
    internal::jniEnv->DeleteGlobalRef(dxEndpointBuilder_);
  }

  DxEndpoint* DxEndpointBuilder::build(JNIEnv* env) {
    auto jDxEndpointBuilderClass = env->GetObjectClass(dxEndpointBuilder_);
    const char* methodName = "build";
    const char* methodSignature = "()Lcom/dxfeed/api/DXEndpoint;";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointBuilderClass, methodName, methodSignature);
    jobject jDxEndpoint = env->CallObjectMethod(dxEndpointBuilder_, methodId);
    javaLogger->info("DxEndpoint OBJECT: %", jDxEndpoint);
    auto* pEndpoint = new DxEndpoint(env, jDxEndpoint);
    env->DeleteLocalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpointBuilderClass);
    return pEndpoint;
  }

  void DxEndpointBuilder::withRole(JNIEnv* env, dxfg_endpoint_role_t role) {
    auto jDxEndpointJniClass = safeFindClass(env, DxEndpoint::DX_JNI_ENDPOINT_CLASS_NAME);
    const char* methodName = "buildWithRole";
    const char* methodSignature = "(Lcom/dxfeed/api/DXEndpoint$Builder;I)Lcom/dxfeed/api/DXEndpoint$Builder;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    jobject jNewBuilder = env->CallStaticObjectMethod(jDxEndpointJniClass, methodId, dxEndpointBuilder_, role);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, jNewBuilder);
    env->DeleteLocalRef(jNewBuilder);
    env->DeleteLocalRef(jDxEndpointJniClass);
  }

  void DxEndpointBuilder::withName(JNIEnv* env, const char* name) {
    auto jDxEndpointBuilderClass = env->GetObjectClass(dxEndpointBuilder_);
    const char* methodName = "withName";
    const char* methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointBuilderClass, methodName, methodSignature);
    jstring jName = env->NewStringUTF(name);
    jobject jNewBuilder = env->CallObjectMethod(dxEndpointBuilder_, methodId, jName);
    env->DeleteLocalRef(jName);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, jNewBuilder);
    env->DeleteLocalRef(jNewBuilder);
    env->DeleteLocalRef(jDxEndpointBuilderClass);
  }

  void DxEndpointBuilder::withProperty(JNIEnv* env, const char* key, const char* value) {
    auto jDxEndpointBuilderClass = env->GetObjectClass(dxEndpointBuilder_);
    const char* methodName = "withProperty";
    const char* methodSignature = "(Ljava/lang/String;Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointBuilderClass, methodName, methodSignature);
    jstring jKey = env->NewStringUTF(key);
    jstring jValue = env->NewStringUTF(value);
    jobject jNewBuilder = env->CallObjectMethod(dxEndpointBuilder_, methodId, jKey, jValue);
    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(jValue);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, jNewBuilder);
    env->DeleteLocalRef(jNewBuilder);
    env->DeleteLocalRef(jDxEndpointBuilderClass);
  }

  void DxEndpointBuilder::withProperties(JNIEnv* env, const char* filePath) {
    jclass jPropertiesClass = safeFindClass(env, "Ljava/util/Properties;");
    javaLogger->info("jPropertiesClass: %", jPropertiesClass);
    const char* methodName = "load";
    const char* methodSignature = "(Ljava/io/InputStream;)V";
    jmethodID loadId = safeGetMethodID(env, jPropertiesClass, methodName, methodSignature);    
    env->DeleteLocalRef(jPropertiesClass);
    //java/util/Properties;
  }

  bool DxEndpointBuilder::supportsProperty(JNIEnv* env, const char* key) {
    auto jDxEndpointBuilderClass = env->GetObjectClass(dxEndpointBuilder_);
    const char* methodName = "supportsProperty";
    const char* methodSignature = "(Ljava/lang/String;)Z";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointBuilderClass, methodName, methodSignature);
    jstring jKey = env->NewStringUTF(key);
    jboolean result = env->CallBooleanMethod(dxEndpointBuilder_, methodId, jKey);
    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(jDxEndpointBuilderClass);
    return result;
  }

  jobject DxEndpointBuilder::rebuild(JNIEnv* env, jobject oldBuilder, jobject newBuilder) {
    if (newBuilder != nullptr) {
      javaLogger->info("newBuilder = %", newBuilder);
      env->DeleteGlobalRef(oldBuilder);
      return env->NewGlobalRef(newBuilder);
    } else {
      javaLogger->error("Can't build DxEndpointBuilder!");
      return oldBuilder;
    }
  }
  
}