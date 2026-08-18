load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

# 1. Заголовки MLIR Lite
cc_library(
    name = "mlir_lite_headers",
    hdrs = glob(["compiler/mlir/lite/**/*.h"]),
    include_prefix = "tensorflow",
    strip_include_prefix = "",
    visibility = ["//visibility:public"],
    deps = ["//third_party/flatbuffers:flatbuffers"],
)

# 2. Основная библиотека TensorFlow Lite
cc_library(
    name = "tensorflow_lite",
    srcs = ["libtensorflowlite.a"], # <-- ЗАМЕНИЛИ .so НА .a
    hdrs = glob(["**/*.h"], exclude = ["compiler/**"]),
    include_prefix = "tensorflow/lite",
    strip_include_prefix = "",
    visibility = ["//visibility:public"],
    linkstatic = True,              # <-- ВКЛЮЧИЛИ СТАТИЧЕСКУЮ ЛИНКОВКУ
    deps = [
        ":mlir_lite_headers",
        "//third_party/flatbuffers:flatbuffers",
    ],
)

# 3. Делегат XNNPACK
cc_library(
    name = "delegates/xnnpack/xnnpack_delegate",
    hdrs = glob(["delegates/xnnpack/*.h"]) if glob(["delegates/xnnpack/*.h"]) else [],
    include_prefix = "tensorflow/lite",
    strip_include_prefix = "",
    visibility = ["//visibility:public"],
    deps = [":tensorflow_lite"],
)

alias(
    name = "framework",
    actual = ":tensorflow_lite",
)

alias(
    name = "kernels/builtin_ops",
    actual = ":tensorflow_lite",
)

