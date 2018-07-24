load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "grok3d",
    commit = "bazel_migration",
    remote = "https://github.com/brandonpollack23/grok3d",
)

git_repository(
    name = "notstd",
    commit = "increstructure",
    remote = "https://github.com/brandonpollack23/notstd",
)

new_git_repository(
    name = "glm",
    build_file = "glm.BUILD",
    remote = "https://github.com/g-truc/glm",
    tag = "0.9.9.0",
)

new_git_repository(
    name = "glfw3",
    build_file = "glfw3.BUILD",
    remote = "https://github.com/glfw/glfw",
    tag = "3.2.1",
)
