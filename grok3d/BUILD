licenses(["notice"])

package(
    default_visibility = ["//grok3d:__subpackages__"],
)

GROK3D_RUNTIME_LIBS = [
    "-lX11",
    "-lpthread",
    "-lGL",
    "-ldl",
]

# Too much template interdependence etc to have a many sub libraries, this has to be monolithic.
filegroup(
    name = "ecs_files",
    srcs = glob([
        "ecs/**/*.cpp",
        "ecs/**/*.h",
    ]),
    visibility = ["//visibility:private"],
)

filegroup(
    name = "engine_files",
    srcs = glob([
        "engine/**/*.cpp",
        "engine/**/*.h",
    ]),
    visibility = ["//visibility:private"],
)

filegroup(
    name = "shaders_files",
    srcs = glob([
        "shaders/**/*.cpp",
        "shaders/**/*.h",
    ]),
    visibility = ["//visibility:private"],
)

filegroup(
    name = "top_level",
    srcs = [
        "grok3d.h",
        "grok3d_types.h",
    ],
)

cc_library(
    name = "grok3d",
    srcs = [
        ":ecs_files",
        ":engine_files",
        ":shaders_files",
        ":top_level",
    ],
    hdrs = ["grok3d.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//grok3d/glad",
        "//notstd",
        "@glfw//:glfw3",
        "@glm",
    ],
)

# Default test suite of all tests.
test_suite(
    name = "grok3d_tests",
)

# ECS only tests
test_suite(
    name = "ecs_tests",
    tests = [":entityhandle_tests"],
)

cc_test(
    name = "entityhandle_tests",
    srcs = ["tests/ecs/EntityHandleTest.cpp"],
    linkopts = GROK3D_RUNTIME_LIBS,
    deps = [
        ":grok3d",
        "@gtest",
        # Includes the main function for us, custom is possible but not necessary.
        "@gtest//:gtest_main",
    ],
)

#TODO test_suite ecs tests
#TODO test _suite engine tests
#TODO test_suite all tests
