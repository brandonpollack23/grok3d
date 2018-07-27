/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

#include "grok3d/ecs/component/TransformComponent.h"
#include "grok3d/ecs/component/ComponentHandle.h"

using namespace Grok3d;
using namespace Grok3d::Components;

GRK_TransformComponent::GRK_TransformComponent() noexcept :
    m_parent(nullptr),
    m_children(std::vector<GRK_TransformComponent *>()),
    m_localPosition(glm::dvec3(0)),
    m_localScale(glm::dvec3(0)) {
}

auto GRK_TransformComponent::SetParent(GRK_TransformComponent *newParent) -> void {
  m_parent = newParent;
  m_parent->AttachChild(this);
}

auto GRK_TransformComponent::AttachChild(GRK_TransformComponent *newChild) -> void {
  this->m_children.push_back(newChild);
}

auto GRK_TransformComponent::IsChildOf(const GRK_TransformComponent *const possibleParent) const -> bool {
  return possibleParent == m_parent;
}

auto GRK_TransformComponent::GetSiblingIndex() -> std::size_t //use parent and iterate through the list looking for me
{
  return m_parent->GetChildIndex(this);
}

auto GRK_TransformComponent::GetChildIndex(const GRK_TransformComponent *const possibleChild) const -> std::size_t {
  for (size_t i = 0; i < m_children.size(); ++i) {
    if (m_children[i] == possibleChild) {
      return i;
    }
  }
  return -1;
}

auto GRK_TransformComponent::ChildCount() -> int {
  return static_cast<int>(this->m_children.size());
}

auto GRK_TransformComponent::GetWorldPosition() const -> glm::dvec3 {
  //if i have no parent, my position is world position
  //otherwise my position concatonated with my parents' world position (recursive) is my actual position
  if (m_parent == nullptr) {
    return m_localPosition;
  } else {
    return m_localPosition + m_parent->GetWorldPosition();
  }
}

auto GRK_TransformComponent::SetWorldPosition(glm::dvec3 v) -> void {
  //if no parent, setting world position is setting my position
  //otherwise it is these coordinates subtracted from my parent's position recursively
  if (m_parent == nullptr) {
    m_localPosition = v;
  } else {
    m_localPosition = v - m_parent->GetWorldPosition();
  }
}

auto GRK_TransformComponent::SetWorldPosition(const double x, const double y, const double z) -> void {
  //if no parent, setting world position is setting my position
  //otherwise it is these coordinates subtracted from my parent's position recursively
  if (m_parent == nullptr) {
    m_localPosition.x = x;
    m_localPosition.y = y;
    m_localPosition.z = z;
  } else {
    m_localPosition.x = x - m_parent->GetWorldPosition().x;
    m_localPosition.y = y - m_parent->GetWorldPosition().y;
    m_localPosition.z = z - m_parent->GetWorldPosition().z;
  }
}

auto GRK_TransformComponent::GetLocalPosition() const -> glm::dvec3 {
  return m_localPosition;
}

auto GRK_TransformComponent::GetLocalPosition(glm::dvec3 v) -> void {
  this->m_localPosition = v;
}

auto GRK_TransformComponent::TranslateLocal(glm::dvec3 v) -> void {
  this->TranslateLocal(v.x, v.y, v.z);
}

auto GRK_TransformComponent::TranslateLocal(const double x, const double y, const double z) -> void {
  m_localPosition.x += x;
  m_localPosition.y += y;
  m_localPosition.z += z;
}

auto GRK_TransformComponent::GetLocalScale() const -> glm::dvec3 {
  return m_localScale;
}

auto GRK_TransformComponent::SetLocalScale(glm::dvec3 v) -> void {
  m_localScale = v;
}

auto GRK_TransformComponent::SetLocalScale(const double x, const double y, const double z) -> void {
  m_localScale.x = x;
  m_localScale.y = y;
  m_localScale.z = z;
}

auto GRK_TransformComponent::DetachChildren() -> void {
  for (auto child : m_children) {
    child->SetParent(nullptr);
  }
  m_children.clear();
}

auto GRK_TransformComponent::GetChild(const unsigned int index) const -> GRK_TransformComponent * {
  return (index < m_children.size() && index > 0) ? m_children[index] : nullptr;
}

//template specialization for GRK_TransformComponent
//every entity MUST have this it cannot be destroyed
template<>
auto GRK_ComponentHandle<GRK_TransformComponent>::Destroy() const -> GRK_Result {
  //TODO 20 log that this cant happen
  return GRK_Result::Ok;
}

