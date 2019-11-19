// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REALSENSE_RS_PIXEL_FORMAT_H_
#define REALSENSE_RS_PIXEL_FORMAT_H_

#include <librealsense2/rs.hpp>

#include "felicia/drivers/camera/camera_format_message.pb.h"

namespace felicia {
namespace drivers {

PixelFormat FromRs2Format(rs2_format rs2_format);
rs2_format ToRs2Format(PixelFormat pixel_format);

}  // namespace drivers
}  // namespace felicia

#endif  // REALSENSE_RS_PIXEL_FORMAT_H_