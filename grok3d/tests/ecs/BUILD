package(
    default_visibility = ["//grok3d:__subpackages__"],
)

GROK3D_RUNTIME_LIBS = [
    "-lX11",
    "-lpthread",
    "-lGL",
    "-ldl",
]

# ECS only tests
test_suite(
    name = "ecs_tests",
    tests = [
        ":componenthandle_tests",
        ":entityhandle_tests",
        ":gamelogiccomponent_tests",
    ],
)

cc_test(
    name = "entityhandle_tests",
    srcs = ["EntityHandleTest.cpp"],
    linkopts = GROK3D_RUNTIME_LIBS,
    deps = [
        "//grok3d",
        "//grok3d/tests/mocks",
        "@gtest",
        # Includes the main function for us, custom is possible but not necessary.
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "componenthandle_tests",
    srcs = ["componenthandletest.cpp"],
    linkopts = GROK3D_RUNTIME_LIBS,
    deps = [
        "//grok3d",
        "//grok3d/tests/mocks",
        "@gtest",
        # Includes the main function for us, custom is possible but not necessary.
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "gamelogiccomponent_tests",
    srcs = ["gamelogiccomponenttest.cpp"],
    linkopts = GROK3D_RUNTIME_LIBS,
    deps = [
        "//grok3d",
        "//grok3d/tests/mocks",
        "@gtest",
        # Includes the main function for us, custom is possible but not necessary.
        "@gtest//:gtest_main",
    ],
)

#TODO test_suite ecs tests
#TODO test _suite engine tests
#TODO test_suite all tests
