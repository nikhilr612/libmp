SET BAZEL_VC=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC
SET BAZEL_VS=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools
REM Use double slashes
SET PYTHON_BIN_PATH=C:/Users/nikhi/miniconda3/envs/libmp_pyenv/python.exe

REM Optional vars
REM SET BAZEL_VC_FULL_VERSION="14.38.33135"
REM SET BAZEL_WINSDK_FULL_VERSION="10.0.22621.0"

bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/libmp:libmp.dll
