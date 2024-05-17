export BAZEL_VC='C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC'
export BAZEL_VS='C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools'
# Use double slashes
# Note: Bazel requires NumPy (and potentially other packages?)
# A new conda environment with numpy installed (`conda install numpy`) has been tested and works
export PYTHON_BIN_PATH='C:/Users/nikhi/miniconda3/envs/libmp_pyenv/python.exe'

# Optional vars
# export BAZEL_VC_FULL_VERSION="14.33.31629"
# export BAZEL_WINSDK_FULL_VERSION="10.0.19041.0"

bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/libmp:libmp.dll