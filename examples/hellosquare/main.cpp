/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file */

#include "grok3d/grok3d.h"

#include <iostream>

using namespace Grok3d;
using namespace Grok3d::Components;

auto HelloSquareTest(char *args[]) -> void;
std::function<GRK_Result (GRK_EntityComponentManager&)> GetHelloSquareInitializationFunction(const char* const args[]);
auto LoadVertexes() -> std::unique_ptr<float[]>;
std::unique_ptr<unsigned int[]> LoadElementIndices();

auto main(int argc, char *argv[]) -> int {
  if (argc < 3) {
    std::cout << "Triangle test requires a vertex and frag shader passed as arguments 1 and 2" << std::endl;
    return -1;
  } else {
    HelloSquareTest(argv);
  }

  return 0;
}

auto HelloSquareTest(char **args) -> void {
  GRK_Engine engine(GetHelloSquareInitializationFunction(args));
  engine.Run();
}

/* Normally this would be loaded from a file or something, to simulate that and
 * sill allocate memory as you would, I have globals here that I copy into a buffer.
 * File management isn't implemented at the time of writing
 */
float squareFloats[] = {
    0.5f, 0.5f, 0.0f, //top right
    0.5f, -0.5f, 0.0f, //bottom right
    -0.5f, -0.5f, 0.0f, //bottom left
    -0.5f, 0.5f, 0.0f  //top left
};

unsigned int squareIndexes[] = {
    0, 1, 3, //top right triangle
    1, 2, 3  //bottom left triangle
};

/**
 * Returns a function that encloses args and is passed to Grok3d.
 *
 * <p>Grok3d calls this on initialization, passing it it's ECS manager.  You use that ECS manager here to initialize
 * the state of the simulation.  All this does is
 * <ol>
 *  <li>Create an entity</li>
 *  <li>Load vertices and element indexes to render</li>
 *  <li>Loads and compiles a shader program from the named arguments</li>
 *  <li>Creates a render component</li>
 *  <li>Attaches the render component to the entity</li>
 *  <li>Returns the status (should be Ok)<li>
 * <ol>
 * </p>
 *
 * <p>This may seem like a lot, but once there is a fileloader and the user no longer manually specifies vertices,
 * this will just boil down to creating a RenderComponent and attaching it.  The details will be handled by the
 * loader function of configuring the RenderComponent.</p>
 *
 * @param args arguments passed from main contain the strings
 * @return status
 */
std::function<GRK_Result (GRK_EntityComponentManager&)>
GetHelloSquareInitializationFunction(const char *const *args) {
  return [args](GRK_EntityComponentManager &ecm) -> GRK_Result {
    auto triangleEntity = ecm.CreateEntity();

    auto vertexes = LoadVertexes();
    auto indices = LoadElementIndices();
    auto shaderProgram = ShaderManager::ShaderProgram({args[1], args[2]});

    auto rc = GRK_RenderComponent(
        vertexes,
        4,
        sizeof(float),
        GRK_GL_PrimitiveType::Unsigned_Int,
        indices.get(),
        6,
        GRK_OpenGLPrimitive::GL_Triangles,
        shaderProgram.GetId());

    return triangleEntity.AddComponent(std::move(rc));
  };
}

auto LoadElementIndices() -> std::unique_ptr<unsigned int[]> {
  auto indices = std::make_unique<unsigned int[]>(6);
  std::copy(squareIndexes, &squareIndexes[6], indices.get());
  return indices;
}

auto LoadVertexes() -> std::unique_ptr<float[]> {
  auto vertexes = std::make_unique<float[]>(12);
  std::copy(squareFloats, &squareFloats[12], vertexes.get());
  return vertexes;
}
