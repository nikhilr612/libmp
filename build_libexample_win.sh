export BAZEL_VC='C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC'
export BAZEL_VS='C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools'
# Use double slashes
export PYTHON_BIN_PATH='C:\\Users\\rk274\\.conda\\envs\\cpp_mp\\python.exe'

# Optional vars
# export BAZEL_VC_FULL_VERSION="14.33.31629"
# export BAZEL_WINSDK_FULL_VERSION="10.0.19041.0"

bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/libexample:libexample.dll
