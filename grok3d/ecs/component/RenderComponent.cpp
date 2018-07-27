/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file */

#include "grok3d/ecs/component/RenderComponent.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::ShaderManager;

static constexpr unsigned int kDimensions = 3;

// TODO clean
GRK_RenderComponent::GRK_RenderComponent(
    std::unique_ptr<float> vertexes,
    std::size_t vertexCount,
    std::size_t vertexSize, //eg sizeof(float)
    GRK_GL_PrimitiveType indexType,
    void *indices,
    std::size_t numIndices,
    GRK_OpenGLPrimitive primitive,
    ShaderProgram shaderProgram) noexcept :
    vertexes(std::move(vertexes)), // TODO Create another constructor that uses already allocated ones, create alloc manager
    vertexBufferObjectOffset(0),
    vertexCount(vertexCount),
    vertexPrimitiveType(indexType),
    indices(indices),
    numIndices(numIndices),
    elementBufferObject(0), //overwrite later in constructor if necessary
    elementBufferObjectOffset(0),
    drawFunctionType(GRK_DrawFunction::DrawArrays),
    drawingPrimitive(primitive),
    shaderProgram(shaderProgram) {
  //First bind Vertex Array Object, then bind and set vertex buffers, then configure vertex attributes
  glGenVertexArrays(1, &vertexArrayObject); // Create Vertex Array Object.
  glGenBuffers(1, &vertexBufferObject); // Create OGL buffer to store vertex data.

  glBindVertexArray(vertexArrayObject); // Bind this Vertex Array Object to the context.
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); // Bind current Vertex Buffer Object to context's Array Buffer attribute.

  // TODO not always STATIC DRAW.
  // Bound type, size in bytes to copy (3 data per vertex X bytes per data), buffer to copy, data access pattern
  glBufferData(GL_ARRAY_BUFFER, this->vertexCount * vertexSize * kDimensions, this->vertexes.get(), GL_STATIC_DRAW);

  // If we passed any indices we need to set up an Element Buffer Object
  // in order to tell OGL what order to draw the vertexes in to make triangles (this saves
  // repeating vertexes in every triangle)
  if (this->indices != nullptr && this->numIndices > 3 && IndexTypeIsValid()) {
    //create EBO
    glGenBuffers(1, &elementBufferObject);

    // Bind to context and copy index data
    // DO NOT unbind this while an VAO is bound, it is stored in that object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numIndices * SizeOfIndexType(), this->indices, GL_STATIC_DRAW);

    drawFunctionType = GRK_DrawFunction::DrawElements;
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kDimensions * sizeof(float), static_cast<void *>(nullptr));
  glEnableVertexAttribArray(0);

  //we can unbind from array_buffer since the correct buffer is now stored in the VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //unbind VAO just be safe and not change it somewhere later
  glBindVertexArray(0);
}

//GRK_RenderComponent::~GRK_RenderComponent() {
//  glDeleteVertexArrays(1, &vertexArrayObject);
//  glDeleteBuffers(1, &vertexBufferObject);
//
//  if (elementBufferObject != 0) {
//    glDeleteBuffers(1, &elementBufferObject);
//  }
//}

