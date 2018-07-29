/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d/ecs/component/GameLogicComponent.h"

using namespace Grok3d;

GRK_GameLogicComponent::BehaviourHandle GRK_GameLogicComponent::s_nextHandle = 1;

GRK_GameLogicComponent::GRK_GameLogicComponent() noexcept :
    m_behaviours(std::vector<std::unique_ptr<GRK_GameBehaviourBase>>()) {
}

GRK_GameLogicComponent::GRK_GameLogicComponent(GRK_GameLogicComponent &&glc) noexcept {
  if (&glc != this) {
    m_behaviours = std::move(glc.m_behaviours);
    m_behaviourIndexMap = std::move(glc.m_behaviourIndexMap);
  }
}

auto GRK_GameLogicComponent::operator=(GRK_GameLogicComponent &&rhs) noexcept -> GRK_GameLogicComponent & {
  m_behaviours = std::move(rhs.m_behaviours);
  m_behaviourIndexMap = std::move(rhs.m_behaviourIndexMap);
  return *this;
}

auto GRK_GameLogicComponent::Update(double dt) -> void {
  if (m_behavioursToRemove.size() > 0) {
    for (const auto &behaviour : m_behavioursToRemove) {
      UnregisterBehaviour(behaviour);
    }

    m_behavioursToRemove.clear();
  }
  for (const auto &behaviour : m_behaviours) {
    behaviour->Update(dt);
  }
}

auto GRK_GameLogicComponent::RegisterBehaviour(
    std::unique_ptr<GRK_GameBehaviourBase> behaviour) -> GRK_GameLogicComponent::BehaviourHandle {
  behaviour->m_behaviourHandle = s_nextHandle;
  m_behaviours.push_back(std::move(behaviour));
  m_behaviourIndexMap.put(s_nextHandle, (m_behaviours.size() - 1));
  return s_nextHandle++;
}

auto
GRK_GameLogicComponent::EnqueueBehaviourRemoval(const GRK_GameLogicComponent::BehaviourHandle handle) -> GRK_Result {
  m_behavioursToRemove.push_back(handle);

  return GRK_Result::Ok;
}

auto GRK_GameLogicComponent::UnregisterBehaviour(const BehaviourHandle handle) -> GRK_Result {
  if (m_behaviourIndexMap.find(handle) == m_behaviourIndexMap.end()) {
    return GRK_Result::NoSuchElement;
  } else {
    auto removeIndex = m_behaviourIndexMap.at(handle);

    auto backHandle = m_behaviourIndexMap.reverse_at(m_behaviours.size() - 1);

    m_behaviours[removeIndex] = std::move(m_behaviours.back());
    m_behaviours.pop_back();

    m_behaviourIndexMap.erase(handle);

    // If there is nothing in m_behaviours we just erased the last element, so no need to update map
    if (m_behaviours.size() > 0) {
      m_behaviourIndexMap.reverse_erase(m_behaviours.size());
      m_behaviourIndexMap.put(backHandle, removeIndex);
    }

    return GRK_Result::Ok;
  }
}

GRK_GameBehaviourBase::GRK_GameBehaviourBase(GRK_EntityHandle owningEntity) noexcept :
    m_owningEntity(owningEntity) {
}

auto GRK_GameBehaviourBase::UnregisterThisBehaviour() -> GRK_Result {
  return m_owningEntity.GetComponent<GRK_GameLogicComponent>()->EnqueueBehaviourRemoval(m_behaviourHandle);
}
