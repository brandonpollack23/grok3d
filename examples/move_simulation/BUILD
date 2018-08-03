package(default_visibility = ["//visibility:private"])

filegroup(
    name = "sources",
    srcs = glob(["*"]),
)

cc_binary(
    name = "move_simulation",
    srcs = [":sources"],
    linkopts = [
        "-lX11",    
        "-lpthread",    
        "-lGL",
        "-ldl",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//grok3d",
    ],
)
