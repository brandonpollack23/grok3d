/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/grok3d.h"

#include <iostream>

using namespace Grok3d;
using namespace Grok3d::Shaders;

auto CreateVertexAttributes() -> std::tuple<std::unique_ptr<GRK_VertexAttribute[]>, GLsizei>;

static float triangleFloats[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0
};

auto HelloTriangleTest(char** args) -> void {
  auto engineInitialization =
      [args](GRK_EntityComponentManager& ecm) -> GRK_Result {
        auto triangleEntity = ecm.CreateEntity();

        auto vertexes = std::make_unique<float[]>(9);
        std::copy(triangleFloats, &triangleFloats[9], vertexes.get());

        auto shaderProgram = ShaderProgram(args[1], args[2]);

        auto vertexAttributes = CreateVertexAttributes();

        auto rc = GRK_RenderComponent(
            vertexes,
            3,
            sizeof(float),
            GLPrimitives::TextureHandle::NoTexture(),
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

auto main(int argc, char* argv[]) -> int {
  if (argc < 3) {
    std::cout << "Triangle test requires a vertex and frag shader passed as arguments 1 and 2" << std::endl;
    return -1;
  } else {
    HelloTriangleTest(argv);
  }

  return 0;
}

auto CreateVertexAttributes() -> std::tuple<std::unique_ptr<GRK_VertexAttribute[]>, GLsizei> {
  GLsizei numAttributes = 1;
  auto va = std::make_unique<GRK_VertexAttribute[]>(1);
  va[0] = {
      0, // index
      3, // size
      GL_FLOAT, // type
      GL_FALSE, // normalize
      numAttributes * kDimensions * sizeof(float), // stride
      reinterpret_cast<void*>(0)
  };

  return std::make_tuple(std::move(va), numAttributes);
}
