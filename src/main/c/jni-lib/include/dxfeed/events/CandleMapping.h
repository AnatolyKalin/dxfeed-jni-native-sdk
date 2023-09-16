// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_CANDLEMAPPING_H
#define NATIVE_JNI_SDK_CANDLEMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct CandleMapping {
    static dxfg_candle_t* toCandle(const char** pByteData, const double** pDoubleData);
    static void fromCandle(dxfg_candle_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_CANDLEMAPPING_H
