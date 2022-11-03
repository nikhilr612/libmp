# Description:
#   OpenCV libraries for video/image processing on Linux

licenses(["notice"])  # BSD license

exports_files(["LICENSE"])

# ===== NOTES ===== #
#
# The following build rule assumes that OpenCV is installed by
# 'sudo apt-get install libopencv-core-dev libopencv-imgproc-dev libopencv-imgcodecs-dev'
# on Debian Buster/Ubuntu. Alternatively, you can install ALL OpenCV modules using
# `sudo apt-get install libopencv-dev`
#
# The OpenCV imgcodecs module is not required by libexample itself, but rather by
# 1+ of its MediaPipe dependencies.
# If you installed OpenCV differently (e.g., built from source), please modify the
# build rule accordingly.
#
# All paths below are relative to the path defined in /WORKSPACE for "linux_opencv"
# By default this path is /usr, but you can change this if necessary.

cc_library(
    name = "opencv",
    hdrs = glob([
        # For OpenCV 4.x
        #"include/aarch64-linux-gnu/opencv4/opencv2/cvconfig.h",
        #"include/arm-linux-gnueabihf/opencv4/opencv2/cvconfig.h",
        #"include/x86_64-linux-gnu/opencv4/opencv2/cvconfig.h",
        "include/opencv4/opencv2/**/*.h*",
    ]),
    includes = [
        # For OpenCV 4.x
        #"include/aarch64-linux-gnu/opencv4/",
        #"include/arm-linux-gnueabihf/opencv4/",
        #"include/x86_64-linux-gnu/opencv4/",
        "include/opencv4/",
    ],
    linkopts = [
        "-l:libopencv_core.so",
        "-l:libopencv_imgproc.so",
        ## For libexample, we don't need the following OpenCV modules
        # They are unused and only bring unnecessary dependencies
        # such as ffmpeg and mysqlclient
        # Uncomment if you need them
        #"-l:libopencv_calib3d.so",
        #"-l:libopencv_features2d.so",
        #"-l:libopencv_highgui.so",
        #"-l:libopencv_imgcodecs.so",
        #"-l:libopencv_video.so",
        #"-l:libopencv_videoio.so",
    ],
    visibility = ["//visibility:public"],
)
