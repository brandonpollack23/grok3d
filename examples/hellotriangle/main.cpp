/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/grok3d.h"

#include <iostream>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;
using namespace Grok3d::ShaderManager;

static float triangleFloats[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0
};

auto HelloTriangleTest(char *args[]) -> void {
  auto engineInitialization =
      [args](GRK_EntityComponentManager &ecm) -> GRK_Result {
        auto triangleEntity = ecm.CreateEntity();

        auto vertexes = std::make_unique<float[]>(9);
        std::copy(triangleFloats, &triangleFloats[9], vertexes.get());

        auto shaderProgram = ShaderProgram(args[1], args[2]);

        auto rc = GRK_RenderComponent(
            vertexes,
            3,
            sizeof(float),
            GRK_GL_PrimitiveType::Unsigned_Int,
            nullptr,
            0,
            GRK_OpenGLPrimitive::GL_Triangles,
            shaderProgram.GetId());

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
    HelloTriangleTest(argv);
  }

  return 0;
}
