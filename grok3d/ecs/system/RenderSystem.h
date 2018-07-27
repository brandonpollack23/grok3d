/* Copyright (c) 2017 Brandon Pollack
 * * Contact @ grok3d@gmail.com
 * * This file is available under the MIT license included in the project
 * */

/** @file*/

#ifndef __RENDERSYSTEM__H
#define __RENDERSYSTEM__H

#include "grok3d/grok3d_types.h"
#include "grok3d/ecs/component/RenderComponent.h"
#include "grok3d/glad/glad/glad.h"

#include "GLFW/glfw3.h"


namespace Grok3d::Systems {
/**
 * @brief Opengl rendering system which initializes the window and draws
 *
 * @details this system uses glad and glfw to create an opengl window and draw all the
 * GRK_RenderComponents
 * In the future this will be able to be constructed with an enum class which selects rendering
 * backend, OGL, DX, etc*/
class GRK_RenderSystem {
 public:
  using GRK_VertexArrayObject = unsigned int;

  GRK_RenderSystem() noexcept;

  /**Have do all rendering work and have GLFW swap buffers*/
  auto Render() const -> Grok3d::GRK_Result;

  /**initialize m_renderComponents with the store of all render components from the @link
   * Grok3d::GRK_EntityComponentManager GRK_EntityComponentManager @endlink*/
  auto Initialize(GRK_EntityComponentManager *ecm) -> GRK_Result;

 private:
  //TODO move this to input system
  auto ProcessInput() const -> void;

  auto InitializeGLWindow() -> void;

  auto SetGLFWWindowHints() const -> void;

  auto InitializeGLFW() const -> void;

  auto SetGLFWErrorCallback() const -> void;

  auto CreateGLFWWindow() -> void;

  auto ClearBuffer() const -> void;

  auto RenderComponents() const -> void;

  auto Swap() const -> void;

  auto PollWindowEvents() const -> void;

  auto ShouldCloseWindow() const -> int;

  auto CloseRenderingWindow() const -> GRK_Result;

 private:
  bool m_isInitialized; ///< Keeps track if class is initialized.
  const std::vector<Grok3d::Components::GRK_RenderComponent>
      *m_renderComponents; ///< The store of all rendercomponents for quick iterating
  GLFWwindow *m_window; ///< GLFW window context.
};
}

#endif
