EXTERNAL_DEPENDENCIES = [
    "@grok3d//:grok3d",
    "@notstd//:notstd",
]

EXTERNAL_DEPENDENCIES_DEBUG = [
    "@grok3d//:grok3d_debug",
]

CPP_FILES = glob(["src/**/*.cpp"])

HEADER_FILES = glob(["include/**/*.h"])

SHADERS = glob(["shaders/**"])

RUNTIME_FILE_DEPENDENCIES = SHADERS

cc_binary(
    name = "grokgame_debug",
    srcs = CPP_FILES + HEADER_FILES,
    copts = [
        "-std=C++17",
        "-g",
        "-O0",
        "-Wall",
        "-fno-exceptions",
    ],
    data = RUNTIME_FILE_DEPENDENCIES,
    linkopts = [
        "-lglfw",
        "-lGL",
        "-ldl",
    ],
    linkstatic = True,
    deps = EXTERNAL_DEPENDENCIES_DEBUG,
)

cc_binary(
    name = "grokgame",
    srcs = CPP_FILES + HEADER_FILES,
    copts = [
        "-std=C++17",
        "-O3",
        "-Wall",
        "-fno-exceptions",
    ],
    data = RUNTIME_FILE_DEPENDENCIES,
    linkopts = [
        "-lglfw",
        "-lGL",
        "-ldl",
    ],
    linkstatic = True,
    deps = EXTERNAL_DEPENDENCIES,
)
