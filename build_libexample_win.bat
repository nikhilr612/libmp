SET BAZEL_VC=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC
SET BAZEL_VS=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools
REM Use double slashes
SET PYTHON_BIN_PATH=C:\\Users\\rk274\\.conda\\envs\\cpp_mp\\python.exe

REM Optional vars
REM SET BAZEL_VC_FULL_VERSION="14.33.31629"
REM SET BAZEL_WINSDK_FULL_VERSION="10.0.19041.0"

bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/libexample:libexample.dll
