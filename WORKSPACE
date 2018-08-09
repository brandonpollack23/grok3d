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

new_git_repository(
    name = "stb",
    build_file = "buildfiles-bazel/stb.BUILD",
    remote = "https://github.com/nothings/stb",
    commit = "e6afb9cbae4064da8c3e69af3ff5c4629579c1d2", #TODO use branch tracking version
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
