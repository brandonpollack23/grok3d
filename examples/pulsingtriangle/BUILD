package(default_visibility = ["//visibility:private"])

filegroup(
    name = "source",
    srcs = glob([
        "*.h",
        "*.cpp",
    ]),
)

SHADER_FILES = glob(["shaders/**/*"])

cc_binary(
    name = "pulsingtriangle",
    srcs = [":source"],
    data = SHADER_FILES,
    linkopts = [
        "-lX11",
        "-lpthread",
        "-lGL",
        "-ldl",
    ],
    deps = [
        "//grok3d",
        "@glm",
    ],
)
