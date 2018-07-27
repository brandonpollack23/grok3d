/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#ifndef __ENGINE__H
#define __ENGINE__H

#include "grok3d/grok3d_types.h"
#include "grok3d/ecs/system/SystemManager.h"
#include "grok3d/ecs/EntityComponentManager.h"

#include <functional>
#include <chrono>

namespace Grok3d {
struct SimulationTimeValues {
  SimulationTimeValues(std::chrono::nanoseconds simulationTime, std::chrono::nanoseconds accumulator)
      : simulationTime(simulationTime), accumulator(accumulator) {}

  std::chrono::nanoseconds simulationTime;
  std::chrono::nanoseconds accumulator;
};

/**
 * @brief The game engine and main entry point to use Grok3d
 *
 * @details
 * This is the class that contains the
 * @link Grok3d::GRK_EntityComponentManager GRK_EntityComponentManager @endlink
 * and
 * @link Grok3d::Systems::GRK_SystemManager GRK_SystemManager @endlink
 *
 * It works through a relatively simple process:
 * 1) You intialize the state by passing a function that uses a reference to the internal
 * GRK_EntityComponentManager to set up your game/scene
 * 2) You update it in steps of time (for simulation purposes OR you just run it and it loops
 * forever*/
class GRK_Engine {
 public:
  GRK_Engine() noexcept;

  /**Construct and Initialize the engine in the same step
   *
   * @details
   * This works the same as calling the default constructor and subsequently calling
   * @link Grok3d::GRK_Engine::InjectInitialization InjectInitialization @endlink and
   * @link Grok3d::GRK_Engine::Initialize Initialize @endlink*/
  explicit GRK_Engine(std::function<GRK_Result(Grok3d::GRK_EntityComponentManager &)> initFunction) noexcept;

  //TODO overload initialize based on markup file
  /**Run the internally function inserted with
   * @link Grok3d::GRK_Engine::InjectInitialization InjectInitialization @endlink*/
  auto Initialize() -> Grok3d::GRK_Result;

  /**Update all of the systems and advance the simulation forward one step of time
   * @param[in] dt the amount of time to step forward the simulation*/
  auto Update(double dt) -> void;

  /**Draw the scene*/
  auto Render() const -> GRK_Result;

  /**Clean up deleted components*/
  auto GarbageCollect() -> void;

  /**function that never exits and runs update in an infinite loop until broken*/
  auto Run() -> void;

  /**Insert an intialzation function to set up the scene*/
  auto InjectInitialization(
      std::function<Grok3d::GRK_Result(Grok3d::GRK_EntityComponentManager &)> initFunction) -> Grok3d::GRK_Result;

 private:
  auto EnsureInitialized() -> void;

  auto RunGameLoop() -> void;

  auto RunTicks(SimulationTimeValues &simulationTimeValues, std::chrono::nanoseconds tickPeriod) -> void;

 private:
  Grok3d::GRK_EntityComponentManager m_entityComponentManager;
  Grok3d::Systems::GRK_SystemManager m_systemManager;

  /**An init function that sets up the scene by creating entities and adding components
   * using the @link Grok3d::GRK_EntityComponentManager GRK_EntityComponentManager @endlink
   * for an initial state*/
  std::function<Grok3d::GRK_Result(Grok3d::GRK_EntityComponentManager &)> m_initFunction;
};
} /*Grok3d*/

#endif
