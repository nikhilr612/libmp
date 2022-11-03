# Description:
#   OpenCV libraries for video/image processing on Windows

licenses(["notice"])  # BSD license

exports_files(["LICENSE"])

config_setting(
    name = "opt_build",
    values = {"compilation_mode": "opt"},
)

config_setting(
    name = "dbg_build",
    values = {"compilation_mode": "dbg"},
)

# ===== NOTES ===== #
# The following build rule assumes pre-built binaries for OpenCV v4.6.0 were downloaded
# and installed. If you installed OpenCV differently, please modify the build rule accordingly.
#
# All paths below are relative to the path defined in /WORKSPACE for "windows_opencv"
# By default this path is C:\opencv\build (double \), but you can change this if necessary.

OPENCV_VERSION = "460"  # 4.6.0

cc_library(
    name = "opencv",
    srcs = select({
        ":opt_build": [
            "x64/vc15/lib/opencv_world" + OPENCV_VERSION + ".lib",
            "x64/vc15/bin/opencv_world" + OPENCV_VERSION + ".dll",
        ],
        ":dbg_build": [
            "x64/vc15/lib/opencv_world" + OPENCV_VERSION + "d.lib",
            "x64/vc15/bin/opencv_world" + OPENCV_VERSION + "d.dll",
        ],
    }),
    hdrs = glob(["include/opencv2/**/*.h*"]),
    includes = ["include/"],
    linkstatic = 1,
    visibility = ["//visibility:public"],
)
