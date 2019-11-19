// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REALSENSE_RS_CAMERA_FACTORY_H_
#define REALSENSE_RS_CAMERA_FACTORY_H_

#include "chromium/base/macros.h"

#include "realsense/rs_camera.h"
#include "realsense/rs_stream_info.h"

namespace felicia {
namespace drivers {

class RsCameraFactory {
 public:
  static std::unique_ptr<RsCamera> NewDepthCamera(
      const CameraDescriptor& descriptor);

  static Status GetCameraDescriptors(CameraDescriptors* camera_descriptors);

  static Status GetSupportedCapabilities(
      const CameraDescriptor& camera_descriptor,
      RsCapabilityMap* rs_capability_map);

  DISALLOW_COPY_AND_ASSIGN(RsCameraFactory);
};

}  // namespace drivers
}  // namespace felicia

#endif  // REALSENSE_RS_CAMERA_FACTORY_H_