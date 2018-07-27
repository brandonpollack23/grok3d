/* Copyright (c) 2017 Brandon Pollack
 * * Contact @ grok3d@gmail.com
 * * This file is available under the MIT license included in the project
 * */

/** @file*/

#include <iostream>

#include "grok3d/glad/glad/glad.h"
#include "GLFW/glfw3.h"

#include "grok3d/grok3d_types.h"
#include "grok3d/ecs/EntityComponentManager.h"

using namespace Grok3d;
using namespace Grok3d::Systems;
using namespace Grok3d::Components;

constexpr int WINDOW_HEIGHT = 600;
constexpr int WINDOW_WIDTH = 800;

GRK_RenderSystem::GRK_RenderSystem() noexcept :
    m_isInitialized(false) {
}

auto GRK_RenderSystem::Initialize(GRK_EntityComponentManager *ecm) -> GRK_Result {
  m_renderComponents = ecm->GetComponentStore<GRK_RenderComponent>();

  InitializeGLWindow();

  m_isInitialized = true;

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
      [](int error, const char *description) {
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
  m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Grok3d Game", nullptr, nullptr);
  if (m_window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    std::exit(-1);
  }

  glfwMakeContextCurrent(m_window);

  //Initialize GLAD (OGL function loader)
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << "Failed to Initialize GLAD" << std::endl;
    std::exit(-1);
  }

  //set up the callback to adjust viewport on window resize
  glfwSetFramebufferSizeCallback(m_window,
                                 [](GLFWwindow *, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

  //set up the viewport on the OGL context window
  //this is what is used when transforming from normalized device coordinates to actual offset
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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

auto GRK_RenderSystem::ShouldCloseWindow() const -> int { return glfwWindowShouldClose(m_window); }

auto GRK_RenderSystem::ProcessInput() const -> void {
  if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_window, true);
  }
}

auto GRK_RenderSystem::ClearBuffer() const -> void {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

auto GRK_RenderSystem::RenderComponents() const -> void {
  for (auto &renderComponent : *m_renderComponents) {
    // Specify shader program.
    glUseProgram(renderComponent.GetShaderProgram().GetId());

    // Bind VAO (rules for how this vertex shader data is formatted).
    glBindVertexArray(renderComponent.GetVAO());

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
  glfwSwapBuffers(m_window);
}

auto GRK_RenderSystem::PollWindowEvents() const -> void {
  glfwPollEvents();
}

auto GRK_RenderSystem::CloseRenderingWindow() const -> GRK_Result {
  glfwTerminate();
  return GRK_Result::RenderingTerminated;
}
