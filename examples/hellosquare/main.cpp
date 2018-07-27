/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/grok3d.h"

#include <iostream>

using namespace Grok3d;
using namespace Grok3d::Components;

float squareFloats[] = {
    0.5f, 0.5f, 0.0f, //top right
    0.5f, -0.5f, 0.0f, //bottom right
    -0.5f, -0.5f, 0.0f, //bottom left
    -0.5f, 0.5f, 0.0f  //top left
};

auto HelloSquareTest(char *args[]) -> void {
  auto engineInitialization = [args](GRK_EntityComponentManager &ecm) -> GRK_Result {
    auto triangleEntity = ecm.CreateEntity();

    // create vertex array
    auto vertexes = std::make_unique<float>(12);
    std::copy(squareFloats, &squareFloats[12], vertexes.get());

    // create index order array
    unsigned int squareIndexes[] = {
        0, 1, 3, //top right triangle
        1, 2, 3  //bottom left triangle
    };
    auto indices = new unsigned int[6];
    std::copy(squareIndexes, &squareIndexes[6], indices);

    // load shaders
    auto shaderProgram = ShaderManager::ShaderProgram({args[1], args[2]});

    auto rc = GRK_RenderComponent(
        std::move(vertexes),
        4,
        sizeof(float),
        GRK_GL_PrimitiveType::Unsigned_Int,
        indices,
        6,
        GRK_OpenGLPrimitive::GL_Triangles,
        shaderProgram);

    return triangleEntity.AddComponent(std::move(rc));
  };

  GRK_Engine engine(engineInitialization);
  engine.Run();
}

auto main(int argc, char *argv[]) -> int {
  if (argc < 3) {
    std::cout << "Triangle test requires a vertex and frag shader passed as arguments 1 and 2" << std::endl;
    return -1;
  } else {
    HelloSquareTest(argv);
  }

  return 0;
}
