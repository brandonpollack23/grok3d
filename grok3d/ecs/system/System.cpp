/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/grok3d.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;
using namespace Grok3d::Systems;

GRK_System::GRK_System() noexcept {
  m_trackedEntities = std::unordered_set<GRK_EntityHandle>(c_initial_entity_array_size);
}

auto GRK_System::Update(double dt) -> GRK_Result {
  GRK_Result result = GRK_Result::Ok;
  result |= UpdateInternal(dt);
  result |= CompleteUnregisterEntities();
  return result;
}

auto GRK_System::UpdateSystemEntities(const GRK_EntityHandle &entity) -> GRK_Result {
  GRK_ComponentBitMask myMask = GetComponentsBitMask();

  //if mask has all components I need
  if (entity.HasComponents(myMask)) {
    m_trackedEntities.insert(entity);
  } else {
    auto itRemove = m_trackedEntities.find(entity);
    if (itRemove != m_trackedEntities.end()) {
      m_trackedEntities.erase(entity);
    }
  }

  return GRK_Result::Ok;
}

auto GRK_System::UnregisterEntity(const GRK_EntityHandle &entity) -> GRK_Result {
  m_entitiesToUnregister.push_back(entity);

  return GRK_Result::Ok;
}

auto GRK_System::CompleteUnregisterEntities() -> Grok3d::GRK_Result {
  //TODO move from end to make more efficent
  GRK_Result result = GRK_Result::Ok;
  for (auto &entity : m_entitiesToUnregister) {
    auto entityIt = m_trackedEntities.find(entity);
    if (entityIt != m_trackedEntities.end()) {
      m_trackedEntities.erase(entityIt);
    } else {
      result = GRK_Result::NoSuchEntity;
    }
  }

  m_entitiesToUnregister.clear();

  return result;
}
