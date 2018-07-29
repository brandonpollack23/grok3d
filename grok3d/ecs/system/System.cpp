/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/grok3d.h"

using namespace Grok3d;

GRK_System::GRK_System() noexcept {
  trackedEntities_ = std::unordered_set<GRK_EntityHandle>(c_initial_entity_array_size);
}

auto GRK_System::Update(double dt) -> GRK_Result {
  GRK_Result result = GRK_Result::Ok;
  result |= UpdateInternal(dt);
  result |= CompleteUnregisterEntities();
  return result;
}

auto GRK_System::UpdateSystemEntities(const GRK_EntityHandle& entity) -> GRK_Result {
  GRK_ComponentBitMask myMask = GetComponentsBitMask();

  //if mask has all components I need
  if (entity.HasComponents(myMask)) {
    trackedEntities_.insert(entity);
  } else {
    auto itRemove = trackedEntities_.find(entity);
    if (itRemove != trackedEntities_.end()) {
      trackedEntities_.erase(entity);
    }
  }

  return GRK_Result::Ok;
}

auto GRK_System::UnregisterEntity(const GRK_EntityHandle& entity) -> GRK_Result {
  entitiesToUnregister_.push_back(entity);

  return GRK_Result::Ok;
}

auto GRK_System::CompleteUnregisterEntities() -> GRK_Result {
  //TODO move from end to make more efficent
  GRK_Result result = GRK_Result::Ok;
  for (auto& entity : entitiesToUnregister_) {
    auto entityIt = trackedEntities_.find(entity);
    if (entityIt != trackedEntities_.end()) {
      trackedEntities_.erase(entityIt);
    } else {
      result = GRK_Result::NoSuchEntity;
    }
  }

  entitiesToUnregister_.clear();

  return result;
}
