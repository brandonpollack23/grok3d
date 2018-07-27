/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file
 * Definition for render componenent*/

#ifndef __RENDERCOMPONENT__H
#define __RENDERCOMPONENT__H

#include "grok3d/grok3d_types.h"
#include "grok3d/shaders/ShaderManager.h"

#include "grok3d/glad/glad/glad.h"
#include "glm/fwd.hpp"

#include <vector>
#include <memory>

//TODO have unitied "model store" or "file store" so that vertexes can be shared as shared_ptr
namespace Grok3d::Components {
enum class GRK_DrawFunction {
  DrawArrays, ///< Use glDrawArrays to draw this, it is just a list of vertices
  DrawElements ///< Use glDrawElements to draw this, it is an EBO indexed vertex object
};

enum class GRK_OpenGLPrimitive : GLenum {
  GL_Triangles = GL_TRIANGLES
};

class GRK_RenderComponent {
 public:
  GRK_RenderComponent(
      std::unique_ptr<float> vertexes,
      std::size_t vertexCount,
      std::size_t vertexSize,
      GRK_GL_PrimitiveType indexType,
      void *indices,
      std::size_t indexCount,
      GRK_OpenGLPrimitive primitive,
      Grok3d::Utilities::ShaderManager::GRK_ShaderProgramID shaderProgram) noexcept;

  auto GetVAO() const { return m_VAO; }

  auto GetVBOOffset() const { return m_VBOOffset; }

  auto GetVertexCount() const { return m_vertexCount; }

  auto GetIndexType() const { return m_indexType; }

  auto GetIndexCount() const { return m_indexCount; }

  auto GetEBOOffset() const {
    return reinterpret_cast<void *>(SizeOfIndexType() * m_EBOOffset);
  }

  auto GetDrawFunction() const { return m_drawFunction; }

  auto GetPrimitive() const { return static_cast<GLenum>(m_primitive); }

  auto GetShaderProgram() const { return m_shaderProgram; }

  auto SizeOfIndexType() const -> std::size_t {
    switch (m_indexType) {
      case GRK_GL_PrimitiveType::Unsigned_Int:return sizeof(unsigned int);
      case GRK_GL_PrimitiveType::Unsigned_Byte:return sizeof(unsigned char);
      case GRK_GL_PrimitiveType::Unsigned_Short:return sizeof(unsigned short);
    }

    // Unreachable but needed by some compilers.
    return static_cast<size_t>(-1);
  }

 private:
  auto IndexTypeIsValid() const -> bool {
    return m_indexType == GRK_GL_PrimitiveType::Unsigned_Int ||
        m_indexType == GRK_GL_PrimitiveType::Unsigned_Byte ||
        m_indexType == GRK_GL_PrimitiveType::Unsigned_Short;
  }

 private:
  std::unique_ptr<float> m_vertexes;
  std::size_t m_VBOOffset;
  std::size_t m_vertexCount;

  GRK_GL_PrimitiveType m_indexType;
  void *m_indices;
  std::size_t m_indexCount;

  GRK_VertexArrayObject m_VAO; /**< VertexArrayObject descriptor
                                           relates vertex buffer, vertex attributes,
                                           and element buffer in the OGL context*/

  GRK_VertexBufferObject m_VBO; /**< VertexBufferObject descriptor
                                            buffer in the OGL context (on the GPU)
                                            that stores the verticies*/

  GRK_ElementBufferObject m_EBO; /** ElementBufferObject
                                             buffer in OGL context (on the GPU)
                                             that stores the order of the vertices in the VBO
                                             to be rendered in to draw triangles*/

  std::size_t m_EBOOffset;

  GRK_DrawFunction m_drawFunction;

  GRK_OpenGLPrimitive m_primitive;

  Grok3d::Utilities::ShaderManager::GRK_ShaderProgramID m_shaderProgram;
};
}

#endif
