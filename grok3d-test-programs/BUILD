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
    name = "grok3d-test-programs",
    srcs = [":source"],
    linkopts = [
        "-lglfw",
        "-lGL",
        "-ldl",
    ],
    data = [":shader-files"],
    deps = [
        ":shader-files",
        "//grok3d",
        "@glm",
    ],
)
