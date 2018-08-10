/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

#include "grok3d/grok3d.h"

#include <iostream>

// TODO find a different way of disable texture, another flag?

using namespace Grok3d;
using namespace Grok3d::Shaders;
using namespace Grok3d::GLPrimitives;

constexpr float squareVertexArray[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

constexpr unsigned int squareIndexes[] = {
    0, 1, 3, //top right triangle
    1, 2, 3  //bottom left triangle
};


auto HelloTexturedTriangleTest(char** args) -> void;
auto GetEngineInitializationFunction(char* args[]) -> std::function<GRK_Result(GRK_EntityComponentManager&)>;
auto LoadVertexes(std::size_t numVertexes) -> std::unique_ptr<float[]>;
auto LoadElementIndices() -> std::unique_ptr<unsigned int[]>;
auto CreateVertexAttributes() -> std::tuple<std::unique_ptr<GRK_VertexAttribute[]>, GLsizei>;

auto main(int argc, char* argv[]) -> int {
  if (argc < 3) {
    std::cout << "Triangle test requires a vertex and frag shader passed as arguments 1 and 2" << std::endl;
    return -1;
  } else {
    HelloTexturedTriangleTest(argv);
  }

  return 0;
}

void HelloTexturedTriangleTest(char** args) {
  GRK_Engine engine(GetEngineInitializationFunction(args));
  engine.Run();
}

auto GetEngineInitializationFunction(char* args[]) -> std::function<GRK_Result(GRK_EntityComponentManager&)> {
  return [args](GRK_EntityComponentManager& ecm) -> GRK_Result {
    auto triangleEntity = ecm.CreateEntity();

    auto numVertexes = sizeof(squareVertexArray) / sizeof(float);
    auto loadedVertexes = LoadVertexes(numVertexes);
    auto elementIndices = LoadElementIndices();
    auto numIndices = LoadElementIndices();
    auto vertexAttributes = CreateVertexAttributes();

    auto textureHandle = TextureHandle::Load2DTexture("examples/textured_rect/textures/wooden.jpg");

    auto shaderProgram = ShaderProgram(args[1], args[2]);

    triangleEntity.AddComponent(GRK_RenderComponent(
        loadedVertexes,
        numVertexes,
        sizeof(float),
        textureHandle,
        GRK_GL_PrimitiveType::Unsigned_Int,
        elementIndices.get(),
        sizeof(squareIndexes),
        GRK_OpenGLPrimitive::GL_Triangles,
        shaderProgram.GetId(),
        std::get<0>(vertexAttributes).get(),
        std::get<1>(vertexAttributes)));

    return GRK_Result::Ok;
  };
}

std::unique_ptr<float[]> LoadVertexes(std::size_t numVertexes) {
  auto vertexes = std::make_unique<float[]>(numVertexes);
  std::copy(squareVertexArray, &squareVertexArray[numVertexes], vertexes.get());
  return vertexes;
}

auto LoadElementIndices() -> std::unique_ptr<unsigned int[]> {
  auto indices = std::make_unique<unsigned int[]>(6);
  std::copy(squareIndexes, &squareIndexes[6], indices.get());
  return indices;
}

auto CreateVertexAttributes() -> std::tuple<std::unique_ptr<GRK_VertexAttribute[]>, GLsizei> {
  GLsizei numAttributes = 3;
  auto attributes = std::make_unique<GRK_VertexAttribute[]>(static_cast<size_t>(numAttributes));

  GLsizei stride = // 2*size of 3d point float + 1*size of 2d point float
      2 * static_cast<GLsizei>(kDimensions)
          * static_cast<GLsizei>(sizeof(float))
          + static_cast<GLsizei>(sizeof(float) * (kDimensions - 1));

  int colorOffset = kDimensions * sizeof(float);
  int textureCoordinateOffset = colorOffset + kDimensions * sizeof(float);

  attributes[0] = {
      0, // index
      3, // size
      GL_FLOAT, // type
      GL_FALSE, // normalize
      stride, // stride
      reinterpret_cast<void*>(0) // offset
  };
  attributes[1] = {
      1, // index
      3, // size
      GL_FLOAT, // type
      GL_FALSE, // normalize
      stride, // stride
      reinterpret_cast<void*>(colorOffset),
  };
  attributes[2] = {
      2, // index
      2, // size
      GL_FLOAT, // type
      GL_FALSE, // normalize
      stride, // stride
      reinterpret_cast<void*>(textureCoordinateOffset),
  };

  return std::make_tuple(std::move(attributes), numAttributes);
}
