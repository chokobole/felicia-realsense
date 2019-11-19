# Copyright (c) 2019 The Felicia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

load("@com_github_chokobole_felicia//bazel:felicia_cc.bzl", "fel_cc_native_library")

exports_files([
    "LICENSE",
])

fel_cc_native_library(
    name = "realsense",
    srcs = [
        "realsense/rs_camera.cc",
        "realsense/rs_camera_factory.cc",
        "realsense/rs_capability.cc",
        "realsense/rs_pixel_format.cc",
        "realsense/rs_pixel_format.h",
    ],
    hdrs = [
        "realsense/rs_camera.h",
        "realsense/rs_camera_factory.h",
        "realsense/rs_capability.h",
        "realsense/rs_stream_info.h",
    ],
    deps = [
        "//external:felicia",
        "//external:realsense",
    ],
    visibility = ["//visibility:public"],
)
