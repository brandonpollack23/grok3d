/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/**
 * @file
 * Definition for render component.
 */

#ifndef __RENDERCOMPONENT__H
#define __RENDERCOMPONENT__H

#include "grok3d/grok3d_types.h"
#include "grok3d/shaders/ShaderManager.h"

#include "grok3d/glad/glad/glad.h"
#include "glm/fwd.hpp"

#include <vector>
#include <memory>

namespace Grok3d::Components {
enum class GRK_DrawFunction {
  DrawArrays, ///< Use glDrawArrays to draw this, it is just a list of vertices
  DrawElements ///< Use glDrawElements to draw this, it is an EBO indexed vertex object
};

enum class GRK_OpenGLPrimitive : GLenum {
  GL_Triangles = GL_TRIANGLES
};

// TODO create a constructor that uses already created array/element buffer objects.
// when this is created, use shared_ptr to reference count them, and move the freeing of those buffers to it's destructor
// using shared_ptr will require overridding it's constructor/destructor to use OGL.

// TODO for model data, only store GL references to vertex buffers

class GRK_RenderComponent {
 public:
  GRK_RenderComponent(
      std::unique_ptr<float> vertexes,
      std::size_t vertexCount,
      std::size_t vertexSize,
      GRK_GL_PrimitiveType indexType,
      void *indices,
      std::size_t numIndices,
      GRK_OpenGLPrimitive primitive,
      Grok3d::ShaderManager::ShaderProgram shaderProgram) noexcept;

//  ~GRK_RenderComponent();

  auto GetVAO() const { return vertexArrayObject; }

  auto GetVBOOffset() const { return vertexBufferObjectOffset; }

  auto GetVertexCount() const { return vertexCount; }

  auto GetVertexPrimitiveType() const { return vertexPrimitiveType; }

  auto GetIndexCount() const { return numIndices; }

  auto GetEBOOffset() const {
    return reinterpret_cast<void *>(SizeOfIndexType() * elementBufferObjectOffset);
  }

  auto GetDrawFunction() const { return drawFunctionType; }

  auto GetPrimitive() const { return static_cast<GLenum>(drawingPrimitive); }

  auto GetShaderProgram() const -> Grok3d::ShaderManager::ShaderProgram { return shaderProgram; }

  auto SizeOfIndexType() const -> std::size_t {
    switch (vertexPrimitiveType) {
      case GRK_GL_PrimitiveType::Unsigned_Int:return sizeof(unsigned int);
      case GRK_GL_PrimitiveType::Unsigned_Byte:return sizeof(unsigned char);
      case GRK_GL_PrimitiveType::Unsigned_Short:return sizeof(unsigned short);
    }

    // Unreachable but needed by some compilers.
    return static_cast<size_t>(-1);
  }

 private:
  auto IndexTypeIsValid() const -> bool {
    return vertexPrimitiveType == GRK_GL_PrimitiveType::Unsigned_Int ||
        vertexPrimitiveType == GRK_GL_PrimitiveType::Unsigned_Byte ||
        vertexPrimitiveType == GRK_GL_PrimitiveType::Unsigned_Short;
  }

 private:
  std::unique_ptr<float> vertexes;
  std::size_t vertexBufferObjectOffset;
  std::size_t vertexCount;

  GRK_GL_PrimitiveType vertexPrimitiveType;
  void *indices;
  std::size_t numIndices;

  GRK_VertexArrayObject vertexArrayObject; /**< VertexArrayObject descriptor
                                           relates vertex buffer, vertex attributes,
                                           and element buffer in the OGL context*/

  GRK_VertexBufferObject vertexBufferObject; /**< VertexBufferObject descriptor
                                            buffer in the OGL context (on the GPU)
                                            that stores the vertices*/

  GRK_ElementBufferObject elementBufferObject; /** ElementBufferObject
                                             buffer in OGL context (on the GPU)
                                             that stores the order of the vertices in the VBO
                                             to be rendered in to draw triangles*/

  std::size_t elementBufferObjectOffset;

  GRK_DrawFunction drawFunctionType;

  GRK_OpenGLPrimitive drawingPrimitive;

  Grok3d::ShaderManager::ShaderProgram shaderProgram;
};
}

#endif
