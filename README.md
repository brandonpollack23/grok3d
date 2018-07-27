# Grok3d - An Open Source Game Engine You can Grok!

This is an attempt to make an Entity Component System (ECS) based game engine from scratch in c++

## Goals

The main goal is to be well documented and grokable.

I want to write a book to accompany this engine that, if read along with the code's documentation
and source (with an appendix that explains each source file and how it fits into the architechture)
you can go from intermediate c++ programmer to..well at least understanding this engine, hopefully
learning some cool c++ concepts along the way.

Another goal, perhaps as important as the first, is to make consuming this engine stupid easy.

As of right now, the main file is still packaged in the same repo for testing called "maintest.cpp"

This file shows how to consume the engine and write a behaviour script

## Concepts introduced so far

* The basics of an ECS
* some basic template meta programming to accomplish things like
    * Compile time type identification
    * Compile time mask generation
    * Compile time rules checking for specializations of template classes
* Heavy use of more modern features from C++11, C++14, and C++17

## Cloning the repository

I have submodules so clone like this

>git clone --recursive https://www.github.com/brandonpollack23/Grok3d

## Getting Documentation

Once I start writing it, I'll include the latest version of the "book" in this repository,
as for the code documentation it's pretty easy

1. Clone the repository
2. cd into the root of the repo
3. run doxygen
4. open docs folder and open whichever one you want, (i like the html one!)
5. Profit? Idk im not funny

## Building

### All Platforms: Dependencies

1. GLFW
on linux just install GLFW from your package manager,
windows you have to get it compile it and add it to the libraries and headers in the solution file
wherever you put it

### Linux

1. cd Grok3d_Windows  (i know this is strange, i haven't gotten around to renaming once i ported it)
2. make

Requires g++, clang doesn't like some c++17 stuff I did, I'm not sure if it's my fault or clang's
and I dont really care

### Windows

You could probably use mingw and follow the linux strategy or just open the sln file in VS and build

## Contributing

not until it is a stable, real, functioning engine that does something...anything
