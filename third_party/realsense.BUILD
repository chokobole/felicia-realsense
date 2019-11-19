# Copyright (c) 2019 The Felicia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

package(default_visibility = ["//visibility:public"])

cc_import(
    name = "realsense_win_lib",
    interface_library = "realsense_lib/realsense2.lib",
    shared_library = "realsense_bin/realsense2.dll",
)

cc_library(
    name = "realsense",
    hdrs = select({
        ":windows": glob([
            "realsense_include/**",
        ]),
        "//conditions:default": [],
    }),
    linkopts = select({
        ":windows": [],
        "//conditions:default": ["-lrealsense2"]
    }),
    deps = select({
        ":windows": [":realsense_win_lib"],
        "//conditions:default": [],
    }),
    includes = select({
        ":windows": ["realsense_include"],
        "//conditions:default": [],
    }),
)

config_setting(
    name = "windows",
    constraint_values = ["@bazel_tools//platforms:windows"],
)
