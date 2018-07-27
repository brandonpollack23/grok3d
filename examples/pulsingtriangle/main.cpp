/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

#include "grok3d/grok3d.h"

#include <iostream>
#include <cmath>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;
using namespace Grok3d::ShaderManager;

// Normally these would be loaded from some file or something, here they're static, but I show what you would
// do if they weren't by allocating the memory anyway.
static float triangleFloats[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0
};

static constexpr auto kFirstUniform = "ourColor";

class FirstUniformShader : public ShaderProgram {
 public:
  FirstUniformShader(
      const char * const vertexShader,
      const char * const fragmentShader,
      const char * const uniform) : ShaderProgram(vertexShader, fragmentShader) {
    firstUniform = static_cast<GRK_UniformID>(glGetUniformLocation(shaderProgramId, uniform));
    if (firstUniform < 0) {
      std::cout << "Error getting uniform \"" << uniform << "\" from shaders: "
                << vertexShader << " " << fragmentShader << std::endl;
      std::exit(-1);
    }
  }

  auto SetFirstUniformFloat(float f) {
    glUniform4f(firstUniform, 0.0f, f, 0.0f, 1.0f);
  }

 private:
  GRK_UniformID firstUniform;
};

class ChangeColorBehaviour : public GRK_GameBehaviourBase {
 public:
  explicit ChangeColorBehaviour(GRK_EntityHandle entity, FirstUniformShader shader) noexcept
      : GRK_GameBehaviourBase(entity),
        renderComponent(entity.GetComponent<GRK_RenderComponent>()),
        shader(shader) {}

  auto Update(double dt) -> void override {
    time = time + dt;

    // Convert running time into sinusoid.
    float greenValue = std::sin(2 * time) / 2.0f + .5f;
    shader.SetFirstUniformFloat(greenValue);
  }

 private:
  GRK_ComponentHandle<GRK_RenderComponent> renderComponent;
  FirstUniformShader shader;
  double time = 0;
};

// TODO clean
auto HelloChangingTriangleTest(char *args[]) -> void {
  auto engineInitialization =
      [args](GRK_EntityComponentManager &ecm) -> GRK_Result {
        auto triangleEntity = ecm.CreateEntity();

        auto vertexes = std::make_unique<float>(9);
        std::copy(triangleFloats, &triangleFloats[9], vertexes.get());

        auto shaderProgram = FirstUniformShader(args[1], args[2], kFirstUniform);

        auto rc = GRK_RenderComponent(
            std::move(vertexes),
            3,
            sizeof(float),
            GRK_GL_PrimitiveType::Unsigned_Int,
            nullptr,
            0,
            GRK_OpenGLPrimitive::GL_Triangles,
            shaderProgram);

        triangleEntity.AddComponent(std::move(rc));

        GRK_GameLogicComponent glc;
        auto changeColorBehaviour = std::make_unique<ChangeColorBehaviour>(triangleEntity, shaderProgram);
        glc.RegisterBehaviour(std::move(changeColorBehaviour));

        triangleEntity.AddComponent(std::move(glc));

        return GRK_Result::Ok;
      };

  GRK_Engine engine(engineInitialization);
  engine.Run();
};

auto main(int argc, char *argv[]) -> int {
  if (argc < 3) {
    std::cout << "Triangle test requires a vertex and frag shader passed as arguments 1 and 2" << std::endl;
    return -1;
  } else {
    HelloChangingTriangleTest(argv);
  }

  return 0;
}
