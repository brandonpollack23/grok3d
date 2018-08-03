local_repository(
    name = "buildfiles",
    path = "buildfiles-bazel"
)

load("@buildfiles//:glfw_workspace_rule.bzl", "get_glfw")
get_glfw(name = "glfw")

new_git_repository(
    name = "glm",
    build_file = "buildfiles-bazel/glm.BUILD",
    remote = "https://github.com/g-truc/glm",
    tag = "0.9.9.0",
)

# Needed by googletest.
http_archive(
     name = "com_google_absl",
     urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"],
     strip_prefix = "abseil-cpp-master",
)

new_git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.0",
    build_file = "buildfiles-bazel/googletest.BUILD"
)
