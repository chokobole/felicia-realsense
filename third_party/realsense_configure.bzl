# Copyright (c) 2019 The Felicia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

_REALSENSE_PATH = "REALSENSE_PATH"
_DEFAULT_PATH_ON_WINDOWS = "c:\\Program Files (x86)\\Intel RealSense SDK 2.0"

def _is_windows(repository_ctx):
    """Returns true if the host operating system is windows."""
    os_name = repository_ctx.os.name.lower()
    return os_name.find("windows") != -1

def _is_x64(repository_ctx):
    result = repository_ctx.execute(["uname", "-m"])
    return result.stdout.strip() == "x86_64"

def _symlink_dir(repository_ctx, src_dir, dest_dir):
    files = repository_ctx.path(src_dir).readdir()
    for src_file in files:
        repository_ctx.symlink(src_file, dest_dir + "/" + src_file.basename)

def _fail(msg):
    """Output failure message when auto configuration fails."""
    fail("Realsense Confiugation Error: %s\n" % msg)

def _get_realsense_path(repository_ctx):
    realsense_path = repository_ctx.os.environ.get(_REALSENSE_PATH)
    if realsense_path == None:
        realsense_path = _DEFAULT_PATH_ON_WINDOWS

    if repository_ctx.path(realsense_path).exists:
        return realsense_path
    else:
        _fail("Failed to find the path for realsense, Did you installed sdk " +
              "by default? or please locate the sdk path setting " +
              "enviornment variable \"REALSENSE_PATH\"")

def _realsense_configure_impl(repository_ctx):
    repository_ctx.symlink(Label("//third_party:realsense.BUILD"), "BUILD")

    if _is_windows(repository_ctx):
        path = _get_realsense_path(repository_ctx)
        realsense_include = path + "/include"
        if _is_x64(repository_ctx):
            realsense_lib = path + "/lib/x64"
            realsense_bin = path + "/bin/x64"
        else:
            realsense_lib = path + "/lib/x86"
            realsense_bin = path + "/bin/x86"

        _symlink_dir(repository_ctx, realsense_include, "realsense_include")
        _symlink_dir(repository_ctx, realsense_lib, "realsense_lib")
        _symlink_dir(repository_ctx, realsense_bin, "realsense_bin")

realsense_configure = repository_rule(
    implementation = _realsense_configure_impl,
    environ = [
        _REALSENSE_PATH,
    ],
)
"""Detects and configure the realsense header and library

Add the following to your WORKSPACE FILE:

```python
realsense_configure(name = "local_config_realsense")
```
Args:
    name: A unique name for the workspace rule.
"""
