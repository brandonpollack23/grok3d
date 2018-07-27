package(default_visibility = ["//visibility:private"])

filegroup(
    name = "source",
    srcs = glob(["src/*"]),
)

cc_binary(
    name = "grok3d-test-programs",
    srcs = [":source"],
    linkopts = [
        "-lglfw3",
        "-lGL",
        "-ldl",
    ],
    deps = [
        "//grok3d",
        "@glm",
    ],
)
