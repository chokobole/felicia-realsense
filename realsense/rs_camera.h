// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Some of implementations are taken and modified from
// https://github.com/IntelRealSense/realsense-ros/blob/development/realsense2_camera/include/base_realsense_node.h

#ifndef REALSENSE_RS_CAMERA_H_
#define REALSENSE_RS_CAMERA_H_

#include <atomic>

#include <librealsense2/rs.hpp>

#include "chromium/base/containers/flat_map.h"
#include "felicia/core/lib/coordinate/coordinate.h"
#include "felicia/core/util/timestamp/timestamper.h"
#include "felicia/drivers/camera/depth_camera_interface.h"
#include "felicia/drivers/imu/imu_filter_factory.h"
#include "felicia/drivers/imu/imu_filter_interface.h"
#include "felicia/drivers/imu/imu_frame.h"
#include "felicia/map/pointcloud.h"

#include "realsense/rs_capability.h"

namespace felicia {
namespace drivers {

class PipelineSyncer : public rs2::asynchronous_syncer {
 public:
  void operator()(rs2::frame f) const { invoke(std::move(f)); }
};

class RsCamera : public DepthCameraInterface {
 public:
  struct NamedFilter {
    enum Name {
      ALIGN,
      COLORIZER,
      DECIMATION_FILTER,
      DISPARITY_TRANSFORM,
      TEMPORAL_FILTER,
      HOLE_FILLING_FILTER,
      SPATIAL_FILTER,
      POINTCLOUD,
      RATES_PRITNER,
      SPATIAL,
      TEMPORAL,
      THRESHOLD,
      UNITS,
      ZERO_ORDER_INVALIDATION,
    };

    NamedFilter(Name name, std::shared_ptr<rs2::filter> filter)
        : name(name), filter(filter) {}

    Name name;
    std::shared_ptr<rs2::filter> filter;
  };

  struct StartParams {
    CameraFormat requested_color_format;
    CameraFormat requested_depth_format;
    ImuFormat requested_gyro_format;
    ImuFormat requested_accel_format;
    ImuFilterFactory::ImuFilterKind imu_filter_kind;
    std::vector<NamedFilter> named_filters;
    base::TimeDelta gyro_interval;
    base::TimeDelta accel_interval;
    base::TimeDelta pointcloud_interval;

    CameraFrameCallback color_frame_callback;
    DepthCameraFrameCallback depth_frame_callback;
    map::PointcloudCallback
        pointcloud_callback;  // Currently only worked when
                              // |synched_frame_callback| is not null.
    ImuFrameCallback imu_frame_callback;
    StatusCallback status_callback;
  };

  ~RsCamera();

  const ImuFormat& accel_format() const { return accel_format_; }

  const ImuFormat& gyro_format() const { return gyro_format_; }

  // DepthCameraInterface methods
  Status Init() override;
  Status Start(const CameraFormat& requested_color_format,
               const CameraFormat& requested_depth_format,
               CameraFrameCallback color_frame_callback,
               DepthCameraFrameCallback depth_frame_callback,
               StatusCallback status_callback) override;
  Status Stop() override;

  Status SetCameraSettings(const CameraSettings& camera_settings) override;
  Status GetCameraSettingsInfo(
      CameraSettingsInfoMessage* camera_settings) override;

  Status Start(const StartParams& params);

  StatusOr<rs2::sensor> sensor(const RsStreamInfo& rs_stream_info);

  Status SetOption(rs2::sensor& sensor, rs2_option option, float value);
  Status GetOption(rs2::sensor& sensor, rs2_option option, float* value);
  Status GetOptionRange(rs2::sensor& sensor, rs2_option option,
                        rs2::option_range* option_range);
  Status GetAllOptions(rs2::sensor& sensor, std::vector<rs2_option>* options);

 private:
  friend class RsCameraFactory;

  RsCamera(const CameraDescriptor& camera_descriptor);

  void GetCameraSetting(rs2::sensor& sensor, rs2_option option,
                        CameraSettingsModeValue* value);
  void GetCameraSetting(rs2::sensor& sensor, rs2_option option,
                        CameraSettingsRangedValue* value);

  void OnFrame(rs2::frame frame);
  void OnImuFrame(rs2::frame frame);

  void HandleVideoFrame(rs2::video_frame frame, base::TimeDelta timestamp);
  void HandlePoints(rs2::points points, base::TimeDelta timestamp,
                    const rs2::frameset& frameset);

  base::Optional<CameraFrame> ConvertToRequestedPixelFormat(
      rs2::video_frame color_frame, PixelFormat requested_pixel_format,
      base::TimeDelta timestamp);
  CameraFrame FromRsColorFrame(rs2::video_frame color_frame,
                               base::TimeDelta timestamp);
  DepthCameraFrame FromRsDepthFrame(rs2::depth_frame depth_frame,
                                    base::TimeDelta timestamp);

  static Status CreateDevice(const CameraDescriptor& camera_descriptor,
                             rs2::device* device);

  static Status CreateCapabilityMap(rs2::device device,
                                    RsCapabilityMap* rs_capability_map);

  rs2::device device_;
  PipelineSyncer syncer_;
  std::vector<NamedFilter> named_filters_;

  CameraFrame cached_color_frame_;
  float depth_scale_;

  base::flat_map<RsStreamInfo, rs2::sensor> sensors_;
  RsCapabilityMap capability_map_;

  ImuFormat gyro_format_;
  ImuFormat accel_format_;
  std::unique_ptr<ImuFilterInterface> imu_filter_;
  ImuFrameCallback imu_frame_callback_;
  map::PointcloudCallback pointcloud_callback_;

  Coordinate coordinate_;
  ThreadSafeTimestamper timestamper_;

  base::TimeDelta last_pointcloud_timestamp_;
  base::TimeDelta pointcloud_interval_;
  base::TimeDelta last_gyro_timestamp_;
  base::TimeDelta gyro_interval_;
  base::TimeDelta last_accel_timestamp_;
  base::TimeDelta accel_interval_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(RsCamera);
};

}  // namespace drivers
}  // namespace felicia

#endif  // REALSENSE_RS_CAMERA_H_