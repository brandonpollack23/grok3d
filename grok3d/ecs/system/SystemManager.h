/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#ifndef __SYSTEMMANAGER__H
#define __SYSTEMMANAGER__H

#include "grok3d/grok3d_types.h"
#include "grok3d/ecs/system/System.h"
#include "grok3d/ecs/system/GameLogicSystem.h"
#include "grok3d/ecs/system/RenderSystem.h"

#include <array>

namespace Grok3d {
/**
 * @brief the manager for all systems to generalize interaction and act as a location for
 * storage of all the system classes*/
class GRK_SystemManager {
 public:
  GRK_SystemManager() noexcept;

  /**Initializes internal class with the ECM and sets up any classes that depend on it
   * internally*/
  auto Initialize(GRK_EntityComponentManager* ecm) -> GRK_Result;

  /**Forward the entity to all systems, they check if the bit mask of that entity mask that of
   * the system's requirments, if so they are added to the queue to be updated every frame*/
  auto UpdateSystemEntities(const GRK_EntityHandle& entity) -> GRK_Result;

  /**Unregisters the entity from all systems, if it is registered*/
  auto UnregisterEntity(const GRK_EntityHandle& entity) -> GRK_Result;

  /**Iterate through all systems and run their update functions*/
  auto UpdateSystems(double dt) -> GRK_Result;

  /**Call Render on the rendering system*/
  auto Render() const -> GRK_Result;

 private:
  bool isInitialized_;

  GRK_EntityComponentManager* ecm_;

  /// An array for easy iteration of all systems, initialized on construction.
  std::array<GRK_System*, 1> systems_;

  GRK_GameLogicSystem gls_;

  GRK_RenderSystem rs_;
};
}

#endif
