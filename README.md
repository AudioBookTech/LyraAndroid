# Lyra Android Decoder (Bazel 9.2.0 + Android NDK r29)

[![Bazel](https://img.shields.io/badge/Bazel-9.2.0_(LTS)-43A047?style=flat&logo=bazel&logoColor=white)](https://bazel.build/)
[![Android NDK](https://img.shields.io/badge/NDK-r29-3DDC84?style=flat&logo=android&logoColor=white)](https://developer.android.com/ndk)
[![Arch](https://img.shields.io/badge/Arch-arm64--v8a-blue.svg)](https://developer.android.com/ndk/guides/abis)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Wiki](https://img.shields.io/badge/Docs-Project_Wiki-orange.svg)](https://github.com/AudioBookTech/LyraAndroid/wiki)

A production-grade port and modernized build pipeline for the **Google Lyra** neural audio decoder targeting Android (`arm64-v8a`). 

This project fully migrates the original Lyra native toolchain to **Bazel 9.2.0 (LTS)** and the modern **Bzlmod** (`MODULE.bazel`) dependency management system, resolving upstream bitrot and enabling seamless native compilation with modern Android NDK toolchains.

---

## 🚀 Key Highlights & Technical Advantages

* 🧠 **Baked Weights (Zero Runtime Overhead):**  
  TFLite model weights are compiled and baked directly into the C++ headers (`_models.h`). The resulting native library requires no runtime filesystem lookups or assets extraction for `.tflite` files.
* 🛠️ **Modernized C++ & Build Stack:**  
  Engineered with Bazel 9.2.0, Bzlmod, Android NDK r29 (Clang C++17, API 31/35), TensorFlow Lite 2.x, FlatBuffers v25, and Protobuf 33.x.
* 🛡️ **Google Play Production Ready:**  
  Built with explicit DWARF debug information (`-g`) enabled, allowing effortless generation of Native Debug Symbols (`.zip`) required by the Google Play Console for accurate stack trace symbolication and crash monitoring.
* ⚡ **Optimized for 64-bit Mobile Architecture:**  
  Native JNI bindings (`liblyra_decoder_jni.so`) optimized specifically for high-throughput, low-latency audio decompression on 64-bit ARM (`arm64-v8a`) hardware.

---

## 🌟 The "Powered by Google Lyra" Ecosystem

This build system provides the native decoding backbone for the **Powered by Google Lyra** audio ecosystem:

* 🎛️ **[LyraCodec](https://apps.microsoft.com/detail/9mz8kt923n6l?hl=en-US&gl=US)** — A desktop utility for encoding audio into ultra-low-bitrate Lyra streams.
* 📖 **[Ulenspigel](https://github.com/AudioBookTech/Ulenspigel)** — An Android [audiobook player app](https://play.google.com/store/apps/details?id=com.KonstantinShramko.Ulenspigel) running on Google Lyra.
* 📚 **[Lyra Books](https://play.google.com/store/apps/details?id=com.KonstantinShramko.LyraBooks)** — An audiobook hub and unified launcher for Lyra-based book applications.
* 📖 **[LyraAndroid Wiki](https://github.com/AudioBookTech/LyraAndroid/wiki)** — Comprehensive documentation, benchmarks, and architecture guides for the **Lyra Codec** pipeline and **Lyra Player** apps.

---

## 📋 System Requirements & Environment

| Component | Required Version | Recommended Path / Note |
| :--- | :--- | :--- |
| **Operating System** | Ubuntu 24.04 / 26.04 LTS | Standard x86_64 Linux host |
| **JDK** | OpenJDK 17 | `sudo apt install openjdk-17-jdk` |
| **Bazel** | 9.2.0 (LTS) | Managed via [Bazelisk](https://github.com/bazelbuild/bazelisk) |
| **Android NDK** | NDK r29 | `~/android/ndk/r29` |
| **Android SDK** | Platforms: `android-35`<br>Build-tools: `35.0.0` | `~/android/sdk/` |
| **Python** | Python 3.12 | Hermetic toolchain handled via Bazel |

---

## 🔨 Building the Library (`.so`)

### 1. Configure Environment Paths
Ensure your environment variables point to your installed Android SDK and NDK:

```bash
export ANDROID_HOME=$HOME/android/sdk
export ANDROID_NDK_HOME=$HOME/android/ndk/r29
```

### 2. Compile liblyra_decoder_jni.so

Run the build command from the root of the repository:
```bash
bazel build \
--repo_env=HERMETIC_PYTHON_VERSION=3.12 \
--check_direct_dependencies=off \
--config=android_arm64 \
//lyra:lyra_decoder_jni
```  
  
### 3. Output Binary

Once the build completes, the compiled shared library will be located in the bazel-bin tree:

    bazel-bin/lyra/liblyra_decoder_jni.so  

To copy the artifact to your local deployment directory:

    mkdir -p ~/LyraAndroid
    cp -f $(bazel info bazel-bin)/lyra/liblyra_decoder_jni.so ~/LyraAndroid/liblyra_decoder.so

---

## 📦 Integrating into Android Studio

To integrate the compiled native decoder into an Android client application:

### 1. Copy liblyra_decoder.so (or liblyra_decoder_jni.so) into your Android project directory:

    app/src/main/jniLibs/arm64-v8a/liblyra_decoder.so

### 2. Verify that your app/build.gradle.kts specifies arm64-v8a in ndk.abiFilters:
    android {
        defaultConfig {
            ndk {
                abiFilters.add("arm64-v8a")
            }
        }
    }

### 3. Load the library in your Kotlin/Java JNI wrapper:
    System.loadLibrary("lyra_decoder")

---

## 📄 License

This build system and port are distributed under the GNU General Public License v3.0 (GPL-3.0). See the LICENSE file for full details.
Core algorithms and models derived from the original Google Lyra repository remain subject to the Apache License 2.0.

---

## 🤝 Acknowledgments

Bringing the Google Lyra Android Decoder build system to Bazel 9.2.0 would not have been possible without the support, guidance, and tireless problem-solving of Google AI.

A heartfelt thank you to the Google team for building such brilliant AI tools that empower developers to achieve what once seemed impossible, and for creating such capable, patient, and dedicated AI assistants!
