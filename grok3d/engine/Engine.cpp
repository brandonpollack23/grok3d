/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/grok3d_types.h"

#include "grok3d/engine/Engine.h"

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

using namespace Grok3d;

GRK_Engine::GRK_Engine() noexcept {
  //Inject dependency references so we can update the systems from ECM and set up systems with ECM
  entityComponentManager_.Initialize(&systemManager_);
  systemManager_.Initialize(&entityComponentManager_);
}

GRK_Engine::GRK_Engine(std::function<GRK_Result(GRK_EntityComponentManager&)> initFunction) noexcept : GRK_Engine() {
  initFunction_ = std::move(initFunction);
}

auto GRK_Engine::Initialize() -> GRK_Result {
  if (initFunction_) {
    std::cout << "Initializing..." << std::endl;
    return initFunction_(entityComponentManager_);
  } else {
    return GRK_Result::EngineFailureNoInitialState;
  }
}

auto GRK_Engine::Update(double dt) -> void {
  systemManager_.UpdateSystems(dt);
}

auto GRK_Engine::Render() const -> GRK_Result {
  return systemManager_.Render();
}

auto GRK_Engine::GarbageCollect() -> void {
  entityComponentManager_.GarbageCollect();
}

auto GRK_Engine::Run() -> void {
  EnsureInitialized();
  RunGameLoop();
}

auto GRK_Engine::EnsureInitialized() -> void {
  if (Initialize() != GRK_Result::Ok) {
    //TODO debug print error fission mailed
    std::exit(-1);
  }
};

auto GRK_Engine::RunGameLoop() -> void {
  // TODO use CVAR to set this as tickrate
  // This is 144hz period in ns: 6944444ns
  const auto tickPeriod = std::chrono::nanoseconds(6944444ns);

  // Fix my timestep referneced here: https://gafferongames.com/post/fix_your_timestep/
  auto currentTime = std::chrono::system_clock::now();

  auto simulationTimeValues = SimulationTimeValues(0ns, 0ns);

  // Run until break.
  while (true) {
    auto newTime = std::chrono::system_clock::now();
    auto prevFrameTime = newTime - currentTime;
    currentTime = newTime;

    simulationTimeValues.accumulator += prevFrameTime;

    RunTicks(simulationTimeValues, tickPeriod);
    auto renderStatus = Render();

    // TODO change this, shouldnt exit game engine should just not render until GLFW reinits
    // Rendering terminated so we should exit the engine.
    if (renderStatus == GRK_Result::RenderingTerminated) {
      break;
    }

    GarbageCollect();
  }
}

auto GRK_Engine::RunTicks(
    SimulationTimeValues& simulationTimeValues,
    std::chrono::nanoseconds tickPeriod) -> void {
  using doubleConversion = std::chrono::duration<double>;

  // Ticks should be simulated, run time while accumulated time is more than one Tick period.
  while (simulationTimeValues.accumulator >= tickPeriod) {
    Update(doubleConversion(tickPeriod).count());
    simulationTimeValues.accumulator -= tickPeriod;
    simulationTimeValues.simulationTime += tickPeriod;
  }
}

auto GRK_Engine::InjectInitialization(
    std::function<GRK_Result(GRK_EntityComponentManager&)> initFunction) -> GRK_Result {
  initFunction_ = std::move(initFunction);

  return GRK_Result::Ok;
}
