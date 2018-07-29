/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/ecs/system/GameLogicSystem.h"
#include "grok3d/ecs/component/GameLogicComponent.h"

using namespace Grok3d;

GRK_GameLogicSystem::GRK_GameLogicSystem() noexcept : GRK_System() {
}

auto GRK_GameLogicSystem::UpdateInternal(const double dt) -> GRK_Result {
  for (const auto& entity : m_trackedEntities) {
    auto logicComponent = entity.GetComponent<GRK_GameLogicComponent>();
    logicComponent->Update(dt);
  }

  return GRK_Result::Ok;
}

auto GRK_GameLogicSystem::GetComponentsBitMask() const -> GRK_ComponentBitMask {
  static GRK_ComponentBitMask mask =
      IndexToMask(GRK_EntityComponentManager::GetComponentTypeAccessIndex<GRK_GameLogicComponent>());

  return mask;
}
