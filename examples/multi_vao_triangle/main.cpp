/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/grok3d.h"

#include <iostream>
#include <tuple>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;
using namespace Grok3d::ShaderManager;

/**
 * This time more than just the points of the triangle will be "loaded", the colors are stored, too.
 * We'll use the VertexAttributes to specify this "stride".
 */
std::unique_ptr<float[]> LoadVertexes(size_t i);
auto CreateVertexAttributes() -> std::tuple<std::unique_ptr<GRK_VertexAttribute[]>, GLsizei>;

float triangleVertexData[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
};

auto MultiVertexAttributeTest(char **args) -> void {
  auto engineInitialization =
      [args](GRK_EntityComponentManager &ecm) -> GRK_Result {

        auto triangleEntity = ecm.CreateEntity();

        auto numVertexes = sizeof(triangleVertexData) / sizeof(float);
        std::unique_ptr<float[]> vertexes = LoadVertexes(numVertexes);

        auto vertexAttributes = CreateVertexAttributes();

        auto shaderProgram = ShaderProgram(args[1], args[2]);

        auto rc = GRK_RenderComponent(
            vertexes,
            numVertexes,
            sizeof(float),
            GRK_GL_PrimitiveType::Unsigned_Int,
            nullptr,
            0,
            GRK_OpenGLPrimitive::GL_Triangles,
            shaderProgram.GetId(),
            std::get<0>(vertexAttributes).get(),
            std::get<1>(vertexAttributes));

        return triangleEntity.AddComponent(std::move(rc));
      };

  GRK_Engine engine(engineInitialization);
  engine.Run();
}

auto CreateVertexAttributes() -> std::tuple<std::unique_ptr<GRK_VertexAttribute[]>, GLsizei> {
  GLsizei numAttributes = 2;
  auto attributes = std::make_unique<GRK_VertexAttribute[]>(2);

  attributes[0] = {
      0, // index
      3, // size
      GL_FLOAT, // type
      GL_FALSE, // normalize
      numAttributes * kDimensions * sizeof(float), // stride
      reinterpret_cast<void*>(0)
  };
  attributes[1] = {
      1, // index
      3, // size
      GL_FLOAT, // type
      GL_FALSE, // normalize
      numAttributes * kDimensions * sizeof(float), // stride
      reinterpret_cast<void*>(kDimensions * sizeof(float))
  };

  return std::make_tuple(std::move(attributes), numAttributes);
}

std::unique_ptr<float[]> LoadVertexes(size_t numVertexes) {
  auto vertexes = std::make_unique<float[]>(numVertexes);
  std::copy(triangleVertexData, &triangleVertexData[numVertexes], vertexes.get());
  return vertexes;
}

auto main(int argc, char *argv[]) -> int {
  if (argc < 3) {
    std::cout << "Triangle test requires a vertex and frag shader passed as arguments 1 and 2" << std::endl;
    return -1;
  } else {
    MultiVertexAttributeTest(argv);
  }

  return 0;
}
