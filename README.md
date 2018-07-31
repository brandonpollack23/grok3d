# Grok3d - An Open Source Game Engine You can Grok!

This is an attempt to make an Entity Component System (ECS) based game engine from scratch in C++

## Why another game engine?

The main goal is to be well documented and "grokable".

I want to write a to accompany this engine that, if read along with the code's documentation
and source (with a readme that explains each source directory's purpose into the architecture)
you can go from intermediate c++ programmer to...well at least understanding this engine, hopefully
learning some cool c++ concepts along the way.

I'll admit at some points I get a bit carried away, especially in th notstd library which contains some template 
metaprogramming.  Don't be alarmed, you don't really need to understand it to learn about game engines!

Another goal, perhaps as important as the first, is to make consuming this engine relatively easy.

There are a number of examples that I developed along with the creation of the engine itself.

## Concepts introduced so far

* The basics of an ECS
* some basic template meta programming to accomplish things like
    * Compile time type identification
    * Compile time mask generation
    * Compile time rules checking for specializations of template classes
* Heavy use of more modern features from C++11, C++14, and C++17
    * Move semantics
    * tuples
    * Almost Always Auto as described by Herb Sutter [here](https://herbsutter.com/2013/08/12/gotw-94-solution-aaa-style-almost-always-auto/)
* Building and project management with Bazel

## Whats still very WIP

Since I'm still actively developing Grok3d, there are a number of things that aren't even close to final.

I still plan on creating things like

* A shader class generator.
    * Reads in GLSL shader programs and generates a class to manage each one.
    * This allows interacting with a shader without heap allocation, since each one would require a map for interacting 
    with uniforms.
    * This idea may be replaced with a simple shader class with a map so the shaders are easier to work with for a client
* File managers for assets (textures/models)
* Although started on a windows machine, it's been a while (since before I migrated to building with Bazel) since I have built it there.
Bazel requires TOOLCHAIN definitions, something I haven't gotten around to doing.

## What works

Mainly drawing triangles and applying shaders to them.  Right now the rendersystem doesn't take into account position,
but I'll add a uniform to the base vertex shader soon enough to do that.

## Cloning the repository

I have a submodule for my custom BUILD files for external dependencies so clone like this:

>git clone --recursive https://www.github.com/brandonpollack23/Grok3d

## Getting Documentation

Once I start writing it, I'll link to the "book" as for the code documentation it's pretty easy

1. Clone the repository
2. cd into the root of the repo
3. run doxygen
4. open docs folder and open whichever one you want, (i like the html one!)
5. Profit? Idk im not funny

## Building

### All Platforms: Dependencies (for now)

1. GLFW
on linux just install GLFW from your package manager,
windows you have to get it compile it and add it to the path somehow.  I suspect chocolate or something like that would
also work.

I plan on writing a custom Bazel BUILD file for this guy so it can be built from source along with the rest, but for now
there's this runtime library dependency.

### Building

>bazel build //...:all

should do it (make sure to install bazel first)

## Development

I use mostly Google style with my own twists.

Some noticeable differences are:
* I use pointer as part of the type not the name.
* Google has no advisory for AAA, so I just go with what feels right.

I mainly use CLion.  It's great and makes C++ development (almost) as easy as Java sometimes, I encourage you to buy a 
license over at [Jetbrains' website](Jetbrains.com).  It's not much and they do great work.

## Contributing

I don't take contributions yet.  Please feel free to email me though at the email listed in the copyrights :D.
