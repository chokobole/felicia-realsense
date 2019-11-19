// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "realsense/rs_capability.h"

#include "chromium/base/logging.h"

namespace felicia {
namespace drivers {

const RsCapability* GetBestMatchedCapability(
    const CameraFormat& requested, const RsCapabilityList& capabilities) {
  DCHECK(!capabilities.empty());
  const RsCapability* best_match = nullptr;
  for (const RsCapability& capability : capabilities) {
    if (capability.is_camera_format) {
      if (!best_match) {
        best_match = &capability;
      } else {
        if (CompareCapability(requested, capability.format.camera_format,
                              best_match->format.camera_format)) {
          best_match = &capability;
        }
      }
    }
  }
  return best_match;
}

const RsCapability* GetBestMatchedCapability(
    const ImuFormat& requested, const RsCapabilityList& capabilities) {
  DCHECK(!capabilities.empty());
  const RsCapability* best_match = nullptr;
  for (const RsCapability& capability : capabilities) {
    if (!capability.is_camera_format) {
      if (!best_match) {
        best_match = &capability;
      } else {
        if (CompareCapability(requested, capability.format.imu_format,
                              best_match->format.imu_format)) {
          best_match = &capability;
        }
      }
    }
  }
  return best_match;
}

}  // namespace drivers
}  // namespace felicia