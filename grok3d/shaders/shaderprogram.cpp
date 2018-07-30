/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#include "grok3d/shaders/shaderprogram.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace Grok3d;

// TODO make this a class with helper functions and clean up.
// TODO then go ahead and make it a real class that returns shared_ptr, destructor for shared ptr unloads the shader
namespace Grok3d::Shaders {

ShaderProgram::ShaderProgram(const char* const vertexShader, const char* const fragmentShader) {
  auto vertexShaderID =
      LoadShaderFile(vertexShader, Shaders::ShaderType::VertexShader);

  auto fragShaderID =
      LoadShaderFile(fragmentShader, Shaders::ShaderType::FragmentShader);

  shaderProgramId_ = CreateShaderProgram({vertexShaderID, fragShaderID});
}

auto ShaderProgram::GetId() const -> GRK_ShaderProgramID {
  return shaderProgramId_;
}

auto LoadShader(const char* const shaderSource, ShaderType type) -> GRK_ShaderID {
  auto id = glCreateShader(static_cast<GLenum>(type));

  glShaderSource(id, 1, &shaderSource, nullptr);
  glCompileShader(id);

  //check if shader compilation is successful, handle if not
  std::array<char, 512> infoLog{};
  auto success = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, nullptr, infoLog.data());
    std::cerr << "Error occurred, failed to load shader\n\t" << infoLog.data() << std::endl;

    glDeleteShader(id); // Don't leak the shader.

    std::exit(-1); //TODO maybe don't exit
  }

  return id;
}

auto LoadShaderFile(const char* const shaderFile, ShaderType type) -> GRK_ShaderID {
  //open the file
  auto fs = std::ifstream(shaderFile);
  if (!fs.is_open()) {
    std::cout << "No such shader file: " << shaderFile << ".  Exiting...";
    std::exit(-1);
  }

  auto shaderSource = std::string(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());

  return LoadShader(shaderSource.c_str(), type);
}

auto UnloadShader(const GRK_ShaderID id) -> GRK_Result {
  glDeleteShader(id);

  auto error = glGetError();
  if (error == GL_INVALID_VALUE) {
    std::cerr << "Attempted to delete shader which was never created" << std::endl;
    return GRK_Result::OpenGLErrorOccurred;
  } else {
    return GRK_Result::Ok;
  }
}

auto CreateShaderProgram(std::initializer_list<GRK_ShaderID> shaders) -> GRK_ShaderProgramID {
  auto id = GRK_ShaderProgramID{glCreateProgram()};

  for (auto shader : shaders) {
    glAttachShader(id, shader);
    auto status = glGetError();
    if (status) {
      std::cerr << "Error attaching shaders to shader program, invalid input?" << std::endl;

      glDeleteProgram(id);

      std::exit(-1);
    }
  }

  glLinkProgram(id);
  auto status = glGetError();
  if (status) {
    std::cerr << "Error linking shader program, invalid input?" << std::endl;

    glDeleteProgram(id);

    std::exit(-1);
  }

  std::array<char, 512> infoLog{};
  glGetProgramiv(id, GL_LINK_STATUS, reinterpret_cast<GLint*>(&status));
  if (!status) {
    glGetProgramInfoLog(id, 512, nullptr, infoLog.data());

    std::cerr << "Error occurred failed to link program\n\t" << infoLog.data() << std::endl;

    glDeleteProgram(id);
  }

  return id;
}

auto GRK_UnloadShaderProgram(GRK_ShaderProgramID id) -> GRK_Result {
  glDeleteProgram(id);

  auto error = glGetError();
  if (error == GL_INVALID_VALUE) {
    std::cerr << "Attempted to delete program which was never created" << std::endl;
    return GRK_Result::OpenGLErrorOccurred;
  } else {
    return GRK_Result::Ok;
  }
}
}
