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
