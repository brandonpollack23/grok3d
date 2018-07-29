/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/ecs/component/GameLogicComponent.h"

using namespace Grok3d;

GRK_GameLogicComponent::BehaviourHandle GRK_GameLogicComponent::s_nextHandle = 1;

GRK_GameLogicComponent::GRK_GameLogicComponent() noexcept :
    behaviours_(std::vector<std::unique_ptr<GRK_GameBehaviourBase>>()) {
}

GRK_GameLogicComponent::GRK_GameLogicComponent(GRK_GameLogicComponent&& glc) noexcept {
  if (&glc != this) {
    behaviours_ = std::move(glc.behaviours_);
    behaviourIndexMap_ = std::move(glc.behaviourIndexMap_);
  }
}

auto GRK_GameLogicComponent::operator=(GRK_GameLogicComponent&& rhs) noexcept -> GRK_GameLogicComponent& {
  behaviours_ = std::move(rhs.behaviours_);
  behaviourIndexMap_ = std::move(rhs.behaviourIndexMap_);
  return *this;
}

auto GRK_GameLogicComponent::Update(double dt) -> void {
  if (behavioursToRemove_.size() > 0) {
    for (const auto& behaviour : behavioursToRemove_) {
      UnregisterBehaviour(behaviour);
    }

    behavioursToRemove_.clear();
  }
  for (const auto& behaviour : behaviours_) {
    behaviour->Update(dt);
  }
}

auto GRK_GameLogicComponent::RegisterBehaviour(
    std::unique_ptr<GRK_GameBehaviourBase> behaviour) -> GRK_GameLogicComponent::BehaviourHandle {
  behaviour->behaviourHandle_ = s_nextHandle;
  behaviours_.push_back(std::move(behaviour));
  behaviourIndexMap_.put(s_nextHandle, (behaviours_.size() - 1));
  return s_nextHandle++;
}

auto
GRK_GameLogicComponent::EnqueueBehaviourRemoval(const GRK_GameLogicComponent::BehaviourHandle handle) -> GRK_Result {
  behavioursToRemove_.push_back(handle);

  return GRK_Result::Ok;
}

auto GRK_GameLogicComponent::UnregisterBehaviour(const BehaviourHandle handle) -> GRK_Result {
  if (behaviourIndexMap_.find(handle) == behaviourIndexMap_.end()) {
    return GRK_Result::NoSuchElement;
  } else {
    auto removeIndex = behaviourIndexMap_.at(handle);

    auto backHandle = behaviourIndexMap_.reverse_at(behaviours_.size() - 1);

    behaviours_[removeIndex] = std::move(behaviours_.back());
    behaviours_.pop_back();

    behaviourIndexMap_.erase(handle);

    // If there is nothing in behaviours_ we just erased the last element, so no need to update map
    if (behaviours_.size() > 0) {
      behaviourIndexMap_.reverse_erase(behaviours_.size());
      behaviourIndexMap_.put(backHandle, removeIndex);
    }

    return GRK_Result::Ok;
  }
}

GRK_GameBehaviourBase::GRK_GameBehaviourBase(GRK_EntityHandle owningEntity) noexcept :
    owningEntity_(owningEntity) {
}

auto GRK_GameBehaviourBase::UnregisterThisBehaviour() -> GRK_Result {
  return owningEntity_.GetComponent<GRK_GameLogicComponent>()->EnqueueBehaviourRemoval(behaviourHandle_);
}
