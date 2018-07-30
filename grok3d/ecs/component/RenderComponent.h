/* Copyright (c) 2018 Brandon Pollack
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

namespace Grok3d {
enum class GRK_DrawFunction {
  DrawArrays, ///< Use glDrawArrays to draw this, it is just a list of vertices
  DrawElements ///< Use glDrawElements to draw this, it is an EBO indexed vertex object
};

enum class GRK_OpenGLPrimitive : GLenum {
  GL_Triangles = GL_TRIANGLES
};

struct GRK_VertexAttribute {
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalize;
  GLsizei stride;
  void* offset;
};

// TODO create a constructor that uses already created array/element buffer objects.
// when this is created, use shared_ptr to reference count them, and move the freeing of those buffers to it's destructor
// using shared_ptr will require overridding it's constructor/destructor to use OGL.

// TODO for model data, only store GL references to vertex buffers

class GRK_RenderComponent {
 public:
  GRK_RenderComponent(
      std::unique_ptr<float[]>& vertexes,
      std::size_t vertexCount,
      std::size_t vertexSize,
      GRK_GL_PrimitiveType indexType,
      unsigned int* indices,
      std::size_t numIndices,
      GRK_OpenGLPrimitive primitive,
      ShaderManager::GRK_ShaderProgramID shaderProgramID,
      GRK_VertexAttribute vertexAttributes[],
      GLsizei numVertexAttributes) noexcept;

  GRK_RenderComponent() = default;
  GRK_RenderComponent(GRK_RenderComponent&& other);
  GRK_RenderComponent& operator=(GRK_RenderComponent&& other) noexcept;

  ~GRK_RenderComponent();

  void freeGlPrimitives();

  auto GetVAO() const { return vertexArrayObject_; }

  auto GetVBOOffset() const { return vertexBufferObjectOffset_; }

  auto GetVertexCount() const { return vertexCount_; }

  auto GetVertexPrimitiveType() const { return vertexPrimitiveType_; }

  auto GetIndexCount() const { return numIndices_; }

  auto GetEBOOffset() const {
    return reinterpret_cast<void*>(SizeOfIndexType() * elementBufferObjectOffset_);
  }

  auto GetDrawFunction() const { return drawFunctionType_; }

  auto GetPrimitive() const { return static_cast<GLenum>(drawingPrimitive_); }

  auto GetShaderProgramID() const -> ShaderManager::GRK_ShaderProgramID { return shaderProgramID_; }

  auto SizeOfIndexType() const -> std::size_t {
    switch (vertexPrimitiveType_) {
      case GRK_GL_PrimitiveType::Unsigned_Int:return sizeof(unsigned int);
      case GRK_GL_PrimitiveType::Unsigned_Byte:return sizeof(unsigned char);
      case GRK_GL_PrimitiveType::Unsigned_Short:return sizeof(unsigned short);
    }

    // Unreachable but needed by some compilers.
    return static_cast<size_t>(-1);
  }

 private:
  auto IndexTypeIsValid() const -> bool {
    return vertexPrimitiveType_ == GRK_GL_PrimitiveType::Unsigned_Int ||
        vertexPrimitiveType_ == GRK_GL_PrimitiveType::Unsigned_Byte ||
        vertexPrimitiveType_ == GRK_GL_PrimitiveType::Unsigned_Short;
  }

 private:
  std::size_t vertexBufferObjectOffset_;
  std::size_t vertexCount_;

  GRK_GL_PrimitiveType vertexPrimitiveType_;
  std::size_t numIndices_;

  /** VertexArrayObject descriptor
   *  relates vertex buffer, vertex attributes,
   *  and element buffer in the OGL context.
   */
  GRK_VertexArrayObject vertexArrayObject_;

  /**
   * VertexBufferObject descriptor
   *  buffer in the OGL context (on the GPU)
   *  that stores the vertices
   */
  GRK_VertexBufferObject vertexBufferObject_;

  /** ElementBufferObject
   *  buffer in OGL context (on the GPU)
   *  that stores the order of the vertices in the VBO
   *  to be rendered in to draw triangles
   */
  GRK_ElementBufferObject elementBufferObject_;

  std::size_t elementBufferObjectOffset_;

  /** Draw either element or arrays. */
  GRK_DrawFunction drawFunctionType_;

  /** Draw triangles, squres, points, etc. */
  GRK_OpenGLPrimitive drawingPrimitive_;

  /** Which shader program to load. */
  ShaderManager::GRK_ShaderProgramID shaderProgramID_;

  // TODO cache the TransformComponent reference here (everyone has one) so it is easy to get in the corresponding TODO
  // in RenderSystem::RenderComponents function.
};
}

#endif
