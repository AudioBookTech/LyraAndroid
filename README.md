# Lyra Android Decoder (Bazel 9.2.0 + Android NDK r29)
A port and build system of the Google Lyra decoder for Android (arm64-v8a), fully adapted for **Bazel 9.2.0 (LTS)** and the **Bzlmod** module system.

## Project Features
* **Baked weights:** Models are embedded directly into the C++ code (_models.h); external access to .tflite files at runtime is not required.
* **Modern stack:** Bazel 9.2.0, Android NDK r29 (API 31/35), Clang C++17, TensorFlow Lite 2.x, FlatBuffers v25, Protobuf 33.x.
* **Google Play Ready:** Enabled DWARF debug information sections (`-g`) to correctly generate Native Debug Symbols in the Play Console.

## System Requirements
1. **OS:** Ubuntu 26.04
2. **Java:** OpenJDK 17
3. **Android NDK R29:**  (path: `~/android/ndk/r29`)
4. **Android SDK:** `~/android/sdk/platforms/android-35`, `~/android/sdk/build-tools/35.0.0`, `~/android/sdk/cmdline-tools/latest`
5. **Bazel 9.2.0:**  (recommended installation via [Bazelisk](https://github.com/bazelbuild/bazelisk))

## Building the library (.so)
Run the command in the repository root:

bazel build --repo_env=HERMETIC_PYTHON_VERSION=3.12 --check_direct_dependencies=off //lyra:lyra_decoder_jni \
--config=android_arm64 && cp -f $(bazel info bazel-bin)/lyra/liblyra_decoder_jni.so /home/konst/LyraAndroid/liblyra_decoder.so

The finished binary will be located at paths: "~/LyraAndroid/liblyra_decoder.so"

EOF
