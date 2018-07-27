package(default_visibility = ["//visibility:private"])

filegroup(
    name = "source",
    srcs = glob(["src/*"]),
)

SHADER_FILES = glob(["shaders/**/*"])

cc_binary(
    name = "grok3d-test-program",
    srcs = [":source"],
    data = SHADER_FILES,
    linkopts = [
        "-lglfw",
        "-lGL",
        "-ldl",
    ],
    deps = [
        "//grok3d",
        "@glm",
    ],
)
