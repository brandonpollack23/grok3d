/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file */

#include "grok3d/ecs/component/RenderComponent.h"
#include "grok3d/textures/texturehandle.h"

using namespace Grok3d;
using namespace Grok3d::Shaders;
using namespace Grok3d::GLPrimitives;

// TODO should there only be one vertex buffer per shader?
// TODO replace vertexes with handle to vertexes like textures have.
GRK_RenderComponent::GRK_RenderComponent(
    std::unique_ptr<float[]>& vertexes,
    std::size_t vertexCount,
    std::size_t vertexSize, //eg sizeof(float)
    const TextureHandle& textureHandle,
    GRK_GL_PrimitiveType indexType,
    unsigned int* indices,
    std::size_t numIndices,
    GRK_OpenGLPrimitive primitive,
    GRK_ShaderProgramID shaderProgramID,
    GRK_VertexAttribute vertexAttributes[],
    GLsizei numVertexAttributes) noexcept
    : vertexBufferObjectOffset_(0),
      vertexCount_(vertexCount),
      textureHandle_(textureHandle),
      vertexPrimitiveType_(indexType),
      numIndices_(numIndices),
      elementBufferObject_(0), //overwrite later in constructor if necessary
      elementBufferObjectOffset_(0),
      drawFunctionType_(GRK_DrawFunction::DrawArrays),
      drawingPrimitive_(primitive),
      shaderProgramID_(shaderProgramID) {
  CreateVertexArrayAndBuffer();
  SendDataToGPU(vertexes, vertexSize);

  // Used for chaining vertices together to make many triangles.
  MaybeCreateElementArrayBuffer(indices);

  ConfigureVertexAttributes(vertexAttributes, numVertexAttributes);

  UnbindBufferAndVertexArray();

}

void GRK_RenderComponent::CreateVertexArrayAndBuffer() {
  // Create Vertex Array Object.
  glGenVertexArrays(1, &vertexArrayObject_);
  // Create OGL buffer to store vertex data.
  glGenBuffers(1, &vertexBufferObject_);

  // Bind this Vertex Array Object to the context.
  glBindVertexArray(vertexArrayObject_);

  // Bind current Vertex Buffer Object to context's Array Buffer attribute.
  glBindBuffer(
      GL_ARRAY_BUFFER,
      vertexBufferObject_);

}

void GRK_RenderComponent::SendDataToGPU(const std::unique_ptr<float[]>& vertexes, size_t vertexSize) {// TODO not always STATIC DRAW.
  // Bound type, size in bytes to copy (3 data per vertex X bytes per data), buffer to copy, data access pattern
  glBufferData(GL_ARRAY_BUFFER, vertexCount_ * vertexSize * kDimensions, vertexes.get(), GL_STATIC_DRAW);
}

void GRK_RenderComponent::MaybeCreateElementArrayBuffer(const unsigned int* indices) {// If we passed any indices_ we need to set up an Element Buffer Object
  if (indices != nullptr && numIndices_ > 3 && IndexTypeIsValid()) {
    //create EBO
    glGenBuffers(1, &elementBufferObject_);

    // Bind to context and copy index data
    // DO NOT unbind this while an VAO is bound, it is stored in that object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices_ * SizeOfIndexType(), indices, GL_STATIC_DRAW);

    drawFunctionType_ = GRK_DrawFunction::DrawElements;
  }
}

/**
 * Store topology information of the vertex buffer (offsets, sizes, strides of
 * different data
 *
 * <p>such as:
 * <ul>
 *    <li>vertex colors
 *    <li>texture coordinates
 *    <li>the texture data
 */
void GRK_RenderComponent::ConfigureVertexAttributes(
    const GRK_VertexAttribute* vertexAttributes,
    GLsizei numVertexAttributes) {
  for (GLsizei i = 0; i < numVertexAttributes; i++) {
    auto& attribute = vertexAttributes[i];
    glVertexAttribPointer(
        attribute.index,
        attribute.size,
        attribute.type,
        attribute.normalize,
        attribute.stride,
        attribute.offset);

    glEnableVertexAttribArray(static_cast<GLuint>(i));
  }
}

void GRK_RenderComponent::UnbindBufferAndVertexArray() {// Buffer already associated with VAO, no need to leave it bound.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Unbind VAO to prevent further modification elsewhere.
  glBindVertexArray(0);
}
