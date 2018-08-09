/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

#ifndef __TEXTUREHANDLE__H
#define __TEXTUREHANDLE__H

#include "grok3d/glad/glad/glad.h"
#include <memory>

// TODO docs

// TODO share texture data with different params?
namespace Grok3d::GLPrimitives {
class TextureHandle {
 public:
  using TextureID = GLuint;

  enum class WrapMode : GLint {
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER,
  };

  enum class FilterMode : GLint {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
    LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR,
  };

  TextureHandle(TextureID id);

  /** Convenience function for repeating textures and trilinear filtering, black border. */
  static auto Load2DTexture(char* filename) -> TextureHandle;
  static auto Load2DTexture(
      char* filename,
      WrapMode sWrapMode,
      WrapMode tWrapMode,
      FilterMode minFilterMode,
      FilterMode magFilterMode,
      float borderColor[4]) -> TextureHandle;

 private:
  static auto Setup2DTextureParameters(
      WrapMode sWrapMode,
      WrapMode tWrapMode,
      FilterMode minFilterMode,
      FilterMode magFilterMode,
      float borderColor[4]) -> void;

  static void SetupTextureWrapping(const WrapMode& sMode, const WrapMode& tMode, const float* borderColor);
  static void Setup2DTextureFiltering(FilterMode minFilterMode, FilterMode magFilterMode);

  static auto Send2DTextureDataToGPU(int width, int height, const std::unique_ptr<unsigned char>& data) -> void;
 private:
  TextureID id_;
  std::shared_ptr<std::nullptr_t> tracker_;
};
}

#endif
