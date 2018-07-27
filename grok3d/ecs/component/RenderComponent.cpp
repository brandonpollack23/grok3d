/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file */

#include "grok3d/ecs/component/RenderComponent.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::ShaderManager;

static constexpr unsigned int c_dimensions = 3;

GRK_RenderComponent::GRK_RenderComponent(
    std::unique_ptr<float> vertexes,
    std::size_t vertexCount,
    std::size_t vertexSize, //eg sizeof(float)
    GRK_GL_PrimitiveType indexType,
    void *indices,
    std::size_t indexCount,
    GRK_OpenGLPrimitive primitive,
    ShaderProgram shaderProgram) noexcept :
    m_vertexes(std::move(vertexes)), // TODO don't store these in cpu memory, only copy to GPU VBO?
    m_VBOOffset(0),
    m_vertexCount(vertexCount),
    m_indexType(indexType),
    m_indices(indices),
    m_indexCount(indexCount),
    m_EBO(0), //overwrite later in constructor if necessary
    m_EBOOffset(0),
    m_drawFunction(GRK_DrawFunction::DrawArrays),
    m_primitive(primitive),
    m_shaderProgram(shaderProgram) {
  //First bind Vertex Array Object, then bind and set vertex buffers, then configure vertex attributes
  glGenVertexArrays(1, &m_VAO); // Create Vertex Array Object.
  glGenBuffers(1, &m_VBO); // Create OGL buffer to store vertex data.
  glBindVertexArray(m_VAO); // Bind this Vertex Array Object to the context.
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind current Vertex Buffer Object to context's Array Buffer attribute.

  // Bound type, size in bytes to copy (3 data per vertex X bytes per data), buffer to copy, data access pattern
  glBufferData(GL_ARRAY_BUFFER, m_vertexCount * vertexSize * c_dimensions, m_vertexes.get(), GL_STATIC_DRAW);

  // If we passed any indices we need to set up an Element Buffer Object
  // in order to tell OGL what order to draw the vertexes in to make triangles (this saves
  // repeating vertexes in every triangle)
  if (m_indices != nullptr && m_indexCount > 3 && IndexTypeIsValid()) {
    //create EBO
    glGenBuffers(1, &m_EBO);

    // Bind to context and copy index data
    // DO NOT unbind this while an VAO is bound, it is stored in that object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * SizeOfIndexType(), m_indices, GL_STATIC_DRAW);

    m_drawFunction = GRK_DrawFunction::DrawElements;
  }

  // TODO make it so we can have more than one vertex attribute, make it so it they be set,
  // removing vertices and these values from constuctor
  // TODO make this configurable
  // Set up vertex attributes for the 0 vertex
  // Configure 0 vertex attribute
  // Size is 3
  // They are floats
  // Do not normalize
  // Stride is 3 floats between each vertex
  // Offset of buffer where vertex data is is 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, c_dimensions * sizeof(float), static_cast<void *>(nullptr));
  glEnableVertexAttribArray(0);

  //we can unbind from array_buffer since the correct buffer is now stored in the VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //unbind VAO just be safe and not change it somewhere later
  glBindVertexArray(0);
}

