# LibMP: MediaPipe as a Shared Library

## Integrate MediaPipe into Your C++ Application

A new MediaPipe build target has been added under [`/mediapipe/examples/desktop/libmp`](/mediapipe/examples/desktop/libmp). Using Bazel, you can build this target to create a shared library (`.so` on Linux, `.dll` on Windows), which you can then integrate - along with its [`.h` header file](/mediapipe/examples/desktop/libmp/libmp.h) - into any C++ application you wish.

Compilation has been tested on Windows (MSVC 17.3.5) and Ubuntu 20.04 (GCC 9.4.0).


## Prerequisites
### Linux
1. Install OpenCV
    - Download pre-compiled binaries: `sudo apt-get install libopencv-core-dev libopencv-imgproc-dev libopencv-imgcodecs-dev`
    - Alternatively, build from source
2. By default, this repository's Bazel build files expect OpenCV to be version 4 and installed using the `apt` package manager (i.e., at `/usr`). To match build settings to your own OpenCV version and path, modify the following Bazel files:
    - `/third_party/opencv_linux.BUILD` (search for "OPENCV_VERSION")
    - `/WORKSPACE` (search for "linux_opencv")
### Windows
1. Install OpenCV
    - Download pre-compiled binaries
        - Visit https://sourceforge.net/projects/opencvlibrary/files
        - Click on folder of desired OpenCV version (e.g., 4.6.0)
        - Download and run the installer `.exe` (e.g., `opencv-4.6.0-vc14_vc15.exe`)
    - Alternatively, build from source
2. By default, this repository's Bazel build files expect OpenCV to be version `4.6.0` and installed at `C:\opencv`. To match build settings to your own OpenCV version and path, modify the following Bazel files:
    - `/third_party/opencv_windows.BUILD` (search for "OPENCV_VERSION")
    - `/WORKSPACE` (search for "windows_opencv")


## Building
### Linux
1. `git clone` this repository and `cd` into it
2. Run `bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/libmp:libmp.so`
### Windows
1. `git clone` this repository and `cd` into it
2. Run `build_libmp_win.sh` (Bash) or `build_libmp_win.bat` (CMD, Powershell)


## Usage
1. The header file that goes with libmp can be found at [`/mediapipe/examples/desktop/libmp/libmp.h`](/mediapipe/examples/desktop/libmp/libmp.h)
2. The library files can be found under
`/bazel-bin/mediapipe/examples/desktop/libmp` as follows:
    - On Linux:
        1. Use the `libmp.so` shared library in your project
    - On Windows:
        1. Copy `libmp.dll` to your binary's output location (or otherwise ensure that it is in your PATH)
        2. Use the `libmp.dll.if.lib` [import library](https://en.wikipedia.org/wiki/Dynamic-link_library#Import_libraries) in your project
        3. Also copy `opencv_world###.dll` to your binary's output location (or otherwise ensure it is in your PATH)
3. In your binary's output location, create a `mediapipe` directory. Copy the `modules` and `models` folders from **`/bazel-bin/mediapipe`** into this new directory. (Be sure to copy from `/bazel-bin/mediapipe`, _NOT_ `/mediapipe`, as the latter contains empty source directories.)


## Notes
- Unused OpenCV libraries are disabled in `/third_party/opencv_linux.BUILD`. If you need them, you can re-enable them to get them linked in the final binary too.
- One recent breaking change to MediaPipe is that models (e.g., `.tflite` files) are no longer included in the repository itself. They are instead hosted on [Google Cloud Storage (GCS)](https://storage.googleapis.com/mediapipe-assets/). If your code is not working for some reason, you can manually clone an older version of [`google/mediapipe`](https://github.com/google/mediapipe) and copy the source `/mediapipe/models` and `/mediapipe/modules` folders over to your binary's location. See [cc6a2f7](https://github.com/google/mediapipe/tree/cc6a2f7af65977248b2a15f471503da2832f583a) for the last versions of these folders before deletion.

## Contributions
Questions, suggestions, issues, pull requests, etc. are all welcome!

---

|layout|title|nav_order|
|-|-|-|
|default|home|1|

![MediaPipe](https://mediapipe.dev/images/mediapipe_small.png)

--------------------------------------------------------------------------------

## Live ML anywhere

[MediaPipe](https://google.github.io/mediapipe/) offers cross-platform, customizable
ML solutions for live and streaming media.

![accelerated.png](https://mediapipe.dev/images/accelerated_small.png)                                                               | ![cross_platform.png](https://mediapipe.dev/images/cross_platform_small.png)
:------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------:
***End-to-End acceleration***: *Built-in fast ML inference and processing accelerated even on common hardware* | ***Build once, deploy anywhere***: *Unified solution works across Android, iOS, desktop/cloud, web and IoT*
![ready_to_use.png](https://mediapipe.dev/images/ready_to_use_small.png)                                                             | ![open_source.png](https://mediapipe.dev/images/open_source_small.png)
***Ready-to-use solutions***: *Cutting-edge ML solutions demonstrating full power of the framework*            | ***Free and open source***: *Framework and solutions both under Apache 2.0, fully extensible and customizable*

## ML solutions in MediaPipe

Face Detection                                                                                                                 | Face Mesh                                                                                                       | Iris                                                                                                      | Hands                                                                                                      | Pose                                                                                                      | Holistic
:----------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------: | :------:
[![face_detection](https://mediapipe.dev/images/mobile/face_detection_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/face_detection) | [![face_mesh](https://mediapipe.dev/images/mobile/face_mesh_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/face_mesh) | [![iris](https://mediapipe.dev/images/mobile/iris_tracking_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/iris) | [![hand](https://mediapipe.dev/images/mobile/hand_tracking_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/hands) | [![pose](https://mediapipe.dev/images/mobile/pose_tracking_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/pose) | [![hair_segmentation](https://mediapipe.dev/images/mobile/holistic_tracking_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/holistic)

Hair Segmentation                                                                                                                       | Object Detection                                                                                                                     | Box Tracking                                                                                                                | Instant Motion Tracking                                                                                                                               | Objectron                                                                                                             | KNIFT
:-------------------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------: | :---:
[![hair_segmentation](https://mediapipe.dev/images/mobile/hair_segmentation_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/hair_segmentation) | [![object_detection](https://mediapipe.dev/images/mobile/object_detection_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/object_detection) | [![box_tracking](https://mediapipe.dev/images/mobile/object_tracking_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/box_tracking) | [![instant_motion_tracking](https://mediapipe.dev/images/mobile/instant_motion_tracking_android_small.gif)](https://google.github.io/mediapipe/solutions/instant_motion_tracking) | [![objectron](https://mediapipe.dev/images/mobile/objectron_chair_android_gpu_small.gif)](https://google.github.io/mediapipe/solutions/objectron) | [![knift](https://mediapipe.dev/images/mobile/template_matching_android_cpu_small.gif)](https://google.github.io/mediapipe/solutions/knift)

<!-- []() in the first cell is needed to preserve table formatting in GitHub Pages. -->
<!-- Whenever this table is updated, paste a copy to solutions/solutions.md. -->

[]()                                                                                      | [Android](https://google.github.io/mediapipe/getting_started/android) | [iOS](https://google.github.io/mediapipe/getting_started/ios) | [C++](https://google.github.io/mediapipe/getting_started/cpp) | [Python](https://google.github.io/mediapipe/getting_started/python) | [JS](https://google.github.io/mediapipe/getting_started/javascript) | [Coral](https://github.com/google/mediapipe/tree/master/mediapipe/examples/coral/README.md)
:---------------------------------------------------------------------------------------- | :-------------------------------------------------------------: | :-----------------------------------------------------: | :-----------------------------------------------------: | :-----------------------------------------------------------: | :-----------------------------------------------------------: | :--------------------------------------------------------------------:
[Face Detection](https://google.github.io/mediapipe/solutions/face_detection)                   | ✅                                                               | ✅                                                       | ✅                                                       | ✅                                                             | ✅                                                             | ✅
[Face Mesh](https://google.github.io/mediapipe/solutions/face_mesh)                             | ✅                                                               | ✅                                                       | ✅                                                       | ✅                                                             | ✅                                                             |
[Iris](https://google.github.io/mediapipe/solutions/iris)                                       | ✅                                                               | ✅                                                       | ✅                                                       |                                                               |                                                               |
[Hands](https://google.github.io/mediapipe/solutions/hands)                                     | ✅                                                               | ✅                                                       | ✅                                                       | ✅                                                             | ✅                                                             |
[Pose](https://google.github.io/mediapipe/solutions/pose)                                       | ✅                                                               | ✅                                                       | ✅                                                       | ✅                                                             | ✅                                                             |
[Holistic](https://google.github.io/mediapipe/solutions/holistic)                               | ✅                                                               | ✅                                                       | ✅                                                       | ✅                                                             | ✅                                                             |
[Selfie Segmentation](https://google.github.io/mediapipe/solutions/selfie_segmentation)         | ✅                                                               | ✅                                                       | ✅                                                       | ✅                                                             | ✅                                                             |
[Hair Segmentation](https://google.github.io/mediapipe/solutions/hair_segmentation)             | ✅                                                               |                                                         | ✅                                                       |                                                               |                                                               |
[Object Detection](https://google.github.io/mediapipe/solutions/object_detection)               | ✅                                                               | ✅                                                       | ✅                                                       |                                                               |                                                               | ✅
[Box Tracking](https://google.github.io/mediapipe/solutions/box_tracking)                       | ✅                                                               | ✅                                                       | ✅                                                       |                                                               |                                                               |
[Instant Motion Tracking](https://google.github.io/mediapipe/solutions/instant_motion_tracking) | ✅                                                               |                                                         |                                                         |                                                               |                                                               |
[Objectron](https://google.github.io/mediapipe/solutions/objectron)                             | ✅                                                               |                                                         | ✅                                                       | ✅                                                             | ✅                                                             |
[KNIFT](https://google.github.io/mediapipe/solutions/knift)                                     | ✅                                                               |                                                         |                                                         |                                                               |                                                               |
[AutoFlip](https://google.github.io/mediapipe/solutions/autoflip)                               |                                                                 |                                                         | ✅                                                       |                                                               |                                                               |
[MediaSequence](https://google.github.io/mediapipe/solutions/media_sequence)                    |                                                                 |                                                         | ✅                                                       |                                                               |                                                               |
[YouTube 8M](https://google.github.io/mediapipe/solutions/youtube_8m)                           |                                                                 |                                                         | ✅                                                       |                                                               |                                                               |

See also
[MediaPipe Models and Model Cards](https://google.github.io/mediapipe/solutions/models)
for ML models released in MediaPipe.

## Getting started

To start using MediaPipe
[solutions](https://google.github.io/mediapipe/solutions/solutions) with only a few
lines code, see example code and demos in
[MediaPipe in Python](https://google.github.io/mediapipe/getting_started/python) and
[MediaPipe in JavaScript](https://google.github.io/mediapipe/getting_started/javascript).

To use MediaPipe in C++, Android and iOS, which allow further customization of
the [solutions](https://google.github.io/mediapipe/solutions/solutions) as well as
building your own, learn how to
[install](https://google.github.io/mediapipe/getting_started/install) MediaPipe and
start building example applications in
[C++](https://google.github.io/mediapipe/getting_started/cpp),
[Android](https://google.github.io/mediapipe/getting_started/android) and
[iOS](https://google.github.io/mediapipe/getting_started/ios).

The source code is hosted in the
[MediaPipe Github repository](https://github.com/google/mediapipe), and you can
run code search using
[Google Open Source Code Search](https://cs.opensource.google/mediapipe/mediapipe).

## Publications

*   [Bringing artworks to life with AR](https://developers.googleblog.com/2021/07/bringing-artworks-to-life-with-ar.html)
    in Google Developers Blog
*   [Prosthesis control via Mirru App using MediaPipe hand tracking](https://developers.googleblog.com/2021/05/control-your-mirru-prosthesis-with-mediapipe-hand-tracking.html)
    in Google Developers Blog
*   [SignAll SDK: Sign language interface using MediaPipe is now available for
    developers](https://developers.googleblog.com/2021/04/signall-sdk-sign-language-interface-using-mediapipe-now-available.html)
    in Google Developers Blog
*   [MediaPipe Holistic - Simultaneous Face, Hand and Pose Prediction, on Device](https://ai.googleblog.com/2020/12/mediapipe-holistic-simultaneous-face.html)
    in Google AI Blog
*   [Background Features in Google Meet, Powered by Web ML](https://ai.googleblog.com/2020/10/background-features-in-google-meet.html)
    in Google AI Blog
*   [MediaPipe 3D Face Transform](https://developers.googleblog.com/2020/09/mediapipe-3d-face-transform.html)
    in Google Developers Blog
*   [Instant Motion Tracking With MediaPipe](https://developers.googleblog.com/2020/08/instant-motion-tracking-with-mediapipe.html)
    in Google Developers Blog
*   [BlazePose - On-device Real-time Body Pose Tracking](https://ai.googleblog.com/2020/08/on-device-real-time-body-pose-tracking.html)
    in Google AI Blog
*   [MediaPipe Iris: Real-time Eye Tracking and Depth Estimation](https://ai.googleblog.com/2020/08/mediapipe-iris-real-time-iris-tracking.html)
    in Google AI Blog
*   [MediaPipe KNIFT: Template-based feature matching](https://developers.googleblog.com/2020/04/mediapipe-knift-template-based-feature-matching.html)
    in Google Developers Blog
*   [Alfred Camera: Smart camera features using MediaPipe](https://developers.googleblog.com/2020/03/alfred-camera-smart-camera-features-using-mediapipe.html)
    in Google Developers Blog
*   [Real-Time 3D Object Detection on Mobile Devices with MediaPipe](https://ai.googleblog.com/2020/03/real-time-3d-object-detection-on-mobile.html)
    in Google AI Blog
*   [AutoFlip: An Open Source Framework for Intelligent Video Reframing](https://ai.googleblog.com/2020/02/autoflip-open-source-framework-for.html)
    in Google AI Blog
*   [MediaPipe on the Web](https://developers.googleblog.com/2020/01/mediapipe-on-web.html)
    in Google Developers Blog
*   [Object Detection and Tracking using MediaPipe](https://developers.googleblog.com/2019/12/object-detection-and-tracking-using-mediapipe.html)
    in Google Developers Blog
*   [On-Device, Real-Time Hand Tracking with MediaPipe](https://ai.googleblog.com/2019/08/on-device-real-time-hand-tracking-with.html)
    in Google AI Blog
*   [MediaPipe: A Framework for Building Perception Pipelines](https://arxiv.org/abs/1906.08172)

## Videos

*   [YouTube Channel](https://www.youtube.com/c/MediaPipe)

## Events

*   [MediaPipe Seattle Meetup, Google Building Waterside, 13 Feb 2020](https://mediapipe.page.link/seattle2020)
*   [AI Nextcon 2020, 12-16 Feb 2020, Seattle](http://aisea20.xnextcon.com/)
*   [MediaPipe Madrid Meetup, 16 Dec 2019](https://www.meetup.com/Madrid-AI-Developers-Group/events/266329088/)
*   [MediaPipe London Meetup, Google 123 Building, 12 Dec 2019](https://www.meetup.com/London-AI-Tech-Talk/events/266329038)
*   [ML Conference, Berlin, 11 Dec 2019](https://mlconference.ai/machine-learning-advanced-development/mediapipe-building-real-time-cross-platform-mobile-web-edge-desktop-video-audio-ml-pipelines/)
*   [MediaPipe Berlin Meetup, Google Berlin, 11 Dec 2019](https://www.meetup.com/Berlin-AI-Tech-Talk/events/266328794/)
*   [The 3rd Workshop on YouTube-8M Large Scale Video Understanding Workshop,
    Seoul, Korea ICCV
    2019](https://research.google.com/youtube8m/workshop2019/index.html)
*   [AI DevWorld 2019, 10 Oct 2019, San Jose, CA](https://aidevworld.com)
*   [Google Industry Workshop at ICIP 2019, 24 Sept 2019, Taipei, Taiwan](http://2019.ieeeicip.org/?action=page4&id=14#Google)
    ([presentation](https://docs.google.com/presentation/d/e/2PACX-1vRIBBbO_LO9v2YmvbHHEt1cwyqH6EjDxiILjuT0foXy1E7g6uyh4CesB2DkkEwlRDO9_lWfuKMZx98T/pub?start=false&loop=false&delayms=3000&slide=id.g556cc1a659_0_5))
*   [Open sourced at CVPR 2019, 17~20 June, Long Beach, CA](https://sites.google.com/corp/view/perception-cv4arvr/mediapipe)

## Community

*   [Awesome MediaPipe](https://mediapipe.page.link/awesome-mediapipe) - A
    curated list of awesome MediaPipe related frameworks, libraries and software
*   [Slack community](https://mediapipe.page.link/joinslack) for MediaPipe users
*   [Discuss](https://groups.google.com/forum/#!forum/mediapipe) - General
    community discussion around MediaPipe

## Alpha disclaimer

MediaPipe is currently in alpha at v0.7. We may be still making breaking API
changes and expect to get to stable APIs by v1.0.

## Contributing

We welcome contributions. Please follow these
[guidelines](https://github.com/google/mediapipe/blob/master/CONTRIBUTING.md).

We use GitHub issues for tracking requests and bugs. Please post questions to
the MediaPipe Stack Overflow with a `mediapipe` tag.
