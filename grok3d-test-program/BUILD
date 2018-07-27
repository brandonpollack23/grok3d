package(default_visibility = ["//visibility:private"])

filegroup(
    name = "source",
    srcs = glob(["src/*"]),
)

filegroup(
    name = "shader-files",
    srcs = glob(["shaders"]),
)

cc_binary(
    name = "grok3d-test-program",
    srcs = [":source"],
    data = [":shader-files"],
    linkopts = [
        "-lglfw",
        "-lGL",
        "-ldl",
    ],
    deps = [
        ":shader-files",
        "//grok3d",
        "@glm",
    ],
)
