/* Copyright (c) 2018 Brandon Pollack
 * * Contact @ grok3d@gmail.com
 * * This file is available under the MIT license included in the project
 * */

/** @file*/

#include <iostream>

#include "grok3d/grok3d_types.h"
#include "grok3d/ecs/system/RenderSystem.h"
#include "grok3d/ecs/EntityComponentManager.h"

#include "grok3d/glad/glad/glad.h"
#include "GLFW/glfw3.h"


using namespace Grok3d;

constexpr int kWindowHeight = 600;
constexpr int kWindowWidth = 800;

GRK_RenderSystem::GRK_RenderSystem() noexcept :
    isInitialized_(false) {
}

auto GRK_RenderSystem::Initialize(GRK_EntityComponentManager* ecm) -> GRK_Result {
  renderComponents_ = ecm->GetComponentStore<GRK_RenderComponent>();

  InitializeGLWindow();

  isInitialized_ = true;

  return GRK_Result::Ok;
}

auto GRK_RenderSystem::InitializeGLWindow() -> void {
  InitializeGLFW();
  SetGLFWWindowHints();
  CreateGLFWWindow();
}

//TODO read a config file for resolution, fullscreen, game name, etc and
auto GRK_RenderSystem::InitializeGLFW() const -> void {
  SetGLFWErrorCallback();

  if (!glfwInit()) {
    std::cerr << "Failed to initialize glfw" << std::endl;
    std::exit(-1);
  }
}

auto GRK_RenderSystem::SetGLFWErrorCallback() const -> void {
  glfwSetErrorCallback(
      [](int error, const char* description) {
        std::cerr << "GLFW error 0x" << std::hex << error << " occured\n";
        std::cerr << description << std::endl;
        std::exit(-1);
      });
}

auto GRK_RenderSystem::SetGLFWWindowHints() const -> void {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}

auto GRK_RenderSystem::CreateGLFWWindow() -> void {
  // TODO CVAR configure width/height/name/fullscreen/etc.
  // Second to last param is nullptr, this is for monitor, we need this when we do fullscreen.
  window_ = glfwCreateWindow(kWindowWidth, kWindowHeight, "Grok3d Game", nullptr, nullptr);
  if (window_ == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    std::exit(-1);
  }

  glfwMakeContextCurrent(window_);

  //Initialize GLAD (OGL function loader)
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << "Failed to Initialize GLAD" << std::endl;
    std::exit(-1);
  }

  //set up the callback to adjust viewport on window resize
  glfwSetFramebufferSizeCallback(window_,
                                 [](GLFWwindow*, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

  //set up the viewport on the OGL context window
  //this is what is used when transforming from normalize device coordinates to actual offset
  glViewport(0, 0, kWindowWidth, kWindowHeight);
}

auto GRK_RenderSystem::Render() const -> GRK_Result {
  //TODO check if initialized
  //TODO move glfw poll events and process input on user input system

  if (!ShouldCloseWindow()) {
    ProcessInput();
    ClearBuffer();
    RenderComponents();
    Swap();
    PollWindowEvents();
    return GRK_Result::Ok;
  } else {
    return CloseRenderingWindow();
  }
}

auto GRK_RenderSystem::ShouldCloseWindow() const -> int { return glfwWindowShouldClose(window_); }

auto GRK_RenderSystem::ProcessInput() const -> void {
  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window_, true);
  }
}

auto GRK_RenderSystem::ClearBuffer() const -> void {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

auto GRK_RenderSystem::RenderComponents() const -> void {
  for (auto& renderComponent : *renderComponents_) {
    // Specify shader program.
    glUseProgram(renderComponent.GetShaderProgramID());

    // TODO for each rendercomponent there can be multiple vertex arrays...
    // Bind VAO (rules for how this vertex shader data is formatted).
    glBindVertexArray(renderComponent.GetVAO());

    // TODO for each rendercomponent it has a transform component...need to find MVP and set uniform.
    // I put a corresponding TODO in RenderComponent.h which caches the transform component, so I can get it easily here.

    //draw
    switch (renderComponent.GetDrawFunction()) {
      case GRK_DrawFunction::DrawArrays:
        glDrawArrays(
            renderComponent.GetPrimitive(),
            renderComponent.GetVBOOffset(),
            renderComponent.GetVertexCount());
        break;
      case GRK_DrawFunction::DrawElements:
        glDrawElements(
            renderComponent.GetPrimitive(),
            renderComponent.GetIndexCount(),
            static_cast<GLenum>(renderComponent.GetVertexPrimitiveType()),
            renderComponent.GetEBOOffset());
        break;
      default:break;
    }
  }
}

/**
 * Swaps back buffer with front buffer, displaying the drawn data.
 *
 * <p>There is an implicit flush that takes place in openGL before swap is done.</p>
 */
auto GRK_RenderSystem::Swap() const -> void {
  glfwSwapBuffers(window_);
}

auto GRK_RenderSystem::PollWindowEvents() const -> void {
  glfwPollEvents();
}

auto GRK_RenderSystem::CloseRenderingWindow() const -> GRK_Result {
  // TODO fix throwing issue
  glfwTerminate();
  return GRK_Result::RenderingTerminated;
}
