// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REALSENSE_RS_CAPABILITY_H_
#define REALSENSE_RS_CAPABILITY_H_

#include <list>

#include "chromium/base/containers/flat_map.h"
#include "felicia/drivers/camera/camera_format.h"
#include "felicia/drivers/imu/imu_format.h"

#include "realsense/rs_stream_info.h"

namespace felicia {
namespace drivers {

struct RsCapability {
  RsCapability(int stream_index, const CameraFormat& format)
      : format(format), stream_index(stream_index), is_camera_format(true) {}

  RsCapability(int stream_index, const ImuFormat& format)
      : format(format), stream_index(stream_index), is_camera_format(false) {}

  RsCapability(const RsCapability& other)
      : stream_index(other.stream_index),
        is_camera_format(other.is_camera_format) {
    if (is_camera_format) {
      format.camera_format = other.format.camera_format;
    } else {
      format.imu_format = other.format.imu_format;
    }
  }

  void operator=(const RsCapability& other) {
    stream_index = other.stream_index;
    is_camera_format = other.is_camera_format;
    if (is_camera_format) {
      format.camera_format = other.format.camera_format;
    } else {
      format.imu_format = other.format.imu_format;
    }
  }

  union Format {
    Format() {}
    Format(const CameraFormat& camera_format) : camera_format(camera_format) {}
    Format(const ImuFormat& imu_format) : imu_format(imu_format) {}

    CameraFormat camera_format;
    ImuFormat imu_format;
  } format;
  int stream_index;
  bool is_camera_format;
};

using RsCapabilityList = std::list<RsCapability>;
using RsCapabilityMap = base::flat_map<RsStreamInfo, RsCapabilityList>;

const RsCapability* GetBestMatchedCapability(
    const CameraFormat& requested, const RsCapabilityList& capabilities);

const RsCapability* GetBestMatchedCapability(
    const ImuFormat& requested, const RsCapabilityList& capabilities);

}  // namespace drivers
}  // namespace felicia

#endif  // REALSENSE_RS_CAPABILITY_H_