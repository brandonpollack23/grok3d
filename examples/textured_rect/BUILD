package(default_visibility = ["//visibility:private"])

SHADER_FILES = glob(["shaders/**/*"])

TEXTURE_FILES = glob(["textures/**/*"])

cc_binary(
    name = "textured_rect",
    srcs = ["main.cpp"],
    data = SHADER_FILES + TEXTURE_FILES,
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
