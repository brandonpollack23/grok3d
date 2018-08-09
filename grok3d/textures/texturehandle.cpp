/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "grok3d/textures/texturehandle.h"
#include "grok3d/glad/glad/glad.h"

#include "stb/stb_image.h"

#include <iostream>

using namespace Grok3d::GLPrimitives;

TextureHandle::TextureHandle(GLuint id)
    : id_(id),
      tracker_(nullptr, [this](std::nullptr_t ptr) {
        glDeleteTextures(1, reinterpret_cast<const GLuint*>(&id_));
      }) {}

auto TextureHandle::Load2DTexture(char* filename) -> TextureHandle {
  float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  return Load2DTexture(
      filename,
      WrapMode::Repeat,
      WrapMode::Repeat,
      FilterMode::LinearMipMapLinear,
      FilterMode::LinearMipMapLinear,
      black);
}

TextureHandle TextureHandle::Load2DTexture(char* filename,
                                          WrapMode sWrapMode,
                                           WrapMode tWrapMode,
                                          FilterMode minFilterMode,
                                          FilterMode magFilterMode,
                                          float* borderColor) {
  // Load texture file image data.
  int width, height, channels_in_file;
  std::unique_ptr<unsigned char> data(stbi_load(filename, &width, &height, &channels_in_file, 0));
  if (data.get() == nullptr) {
    std::cout << "Error loading texture: " << filename
              << "\n" << "Due to: " << stbi_failure_reason() << std::endl;
    std::exit(-1);
  }

  // Create and bind texture in OGL.
  TextureID id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  Setup2DTextureParameters(sWrapMode, tWrapMode, minFilterMode, magFilterMode, borderColor);

  Send2DTextureDataToGPU(width, height, data);

  return TextureHandle(id);
}

auto TextureHandle::Setup2DTextureParameters(
    WrapMode sMode,
    WrapMode tMode,
    FilterMode minFilterMode,
    FilterMode magFilterMode,
    float borderColor[4]) -> void {
  SetupTextureWrapping(sMode, tMode, borderColor);
  Setup2DTextureFiltering(minFilterMode, magFilterMode);
}

void TextureHandle::SetupTextureWrapping(const TextureHandle::WrapMode& sMode,
                                         const TextureHandle::WrapMode& tMode,
                                         const float* borderColor) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(sMode));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(tMode));

  // Set border color regardless.
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void TextureHandle::Setup2DTextureFiltering(
    FilterMode minFilterMode,
    FilterMode magFilterMode) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilterMode));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(magFilterMode));
}

// TODO use file channels to get rgba, grayscale, etc.
void TextureHandle::Send2DTextureDataToGPU(int width, int height, const std::unique_ptr<unsigned char>& data) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
  glGenerateMipmap(GL_TEXTURE_2D);
}
