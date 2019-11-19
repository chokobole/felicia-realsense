# Felicia Realsense

## Prerequisites

Follow the instruction at [Download and Install](https://github.com/IntelRealSense/librealsense#download-and-install).

## WORKSPACE setup

```python
bind(
    name = "felicia",
    actual = "@com_github_chokobole_felicia//felicia:felicia",
)

bind(
    name = "felicia_realsense",
    actual = "@com_github_chokobole_felicia_realsense//:realsense",
)

git_repository(
    name = "com_github_chokobole_felicia",
    remote = "https://github.com/chokobole/felicia.git",
    commit = "<commit>",
)

git_repository(
    name = "com_github_chokobole_felicia_realsense",
    remote = "https://github.com/chokobole/felicia-realsense.git",
    commit = "<commit>",
)

load("@com_github_chokobole_felicia//bazel:felicia_deps.bzl", "felicia_deps")

felicia_deps()

load("@com_github_chokobole_felicia_realsense//:realsense_deps.bzl", "realsense_deps")

realsense_deps()
```

## Example

```python
load("@com_github_chokobole_felicia//bazel:felicia_cc.bzl", "fel_cc_native_binary")

fel_cc_native_binary(
    name = "<name>",
    ...,
    deps = [
        "//external:felicia_realsense",
    ],
)
```

Check out examples [here](https://github.com/chokobole/felicia-examples/blob/master/examples/learn/topic/depth_camera/cc/BUILD).