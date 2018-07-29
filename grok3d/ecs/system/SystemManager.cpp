/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/ecs/system/SystemManager.h"

using namespace Grok3d;

GRK_SystemManager::GRK_SystemManager() noexcept :
    isInitialized_(false),
    ecm_(nullptr),
    gls_(GRK_GameLogicSystem()),
    rs_(GRK_RenderSystem()) {
  systems_ = {&gls_};
}

//TODO fail if not initialized?
auto GRK_SystemManager::Initialize(GRK_EntityComponentManager* ecm) -> GRK_Result {
  //save reference to ecm
  ecm_ = ecm;

  // Render system.
  rs_.Initialize(ecm_);

  isInitialized_ = true;

  return GRK_Result::Ok;
}

auto GRK_SystemManager::UpdateSystemEntities(const GRK_EntityHandle& entity) -> GRK_Result {
  auto result = GRK_Result::Ok;

  for (const auto& system : systems_) {
    result |= system->UpdateSystemEntities(entity);
  }

  return result;
}

auto GRK_SystemManager::UnregisterEntity(const GRK_EntityHandle& entity) -> GRK_Result {
  auto result = GRK_Result::Ok;

  for (auto system : systems_) {
    result |= system->UnregisterEntity(entity);
  }

  return result;
}

auto GRK_SystemManager::UpdateSystems(const double dt) -> GRK_Result {
  auto result = GRK_Result::Ok;
  for (const auto& system : systems_) {
    result |= system->Update(dt);
  }

  return result;
}

auto GRK_SystemManager::Render() const -> GRK_Result {
  return rs_.Render();
}
