/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

#include "grok3d/ecs/component/TransformComponent.h"
#include "grok3d/ecs/component/ComponentHandle.h"

using namespace Grok3d;

GRK_TransformComponent::GRK_TransformComponent() noexcept :
    parent_(nullptr),
    children_(std::vector<GRK_TransformComponent*>()),
    localPosition_(glm::dvec3(0)),
    localScale_(glm::dvec3(0)) {
}

auto GRK_TransformComponent::SetParent(GRK_TransformComponent* newParent) -> void {
  parent_ = newParent;
  parent_->AttachChild(this);
}

auto GRK_TransformComponent::AttachChild(GRK_TransformComponent* newChild) -> void {
  this->children_.push_back(newChild);
}

auto GRK_TransformComponent::IsChildOf(const GRK_TransformComponent* const possibleParent) const -> bool {
  return possibleParent == parent_;
}

auto GRK_TransformComponent::GetSiblingIndex() -> std::size_t //use parent and iterate through the list looking for me
{
  return parent_->GetChildIndex(this);
}

auto GRK_TransformComponent::GetChildIndex(const GRK_TransformComponent* const possibleChild) const -> std::size_t {
  for (size_t i = 0; i < children_.size(); ++i) {
    if (children_[i] == possibleChild) {
      return i;
    }
  }
  return -1;
}

auto GRK_TransformComponent::ChildCount() -> int {
  return static_cast<int>(this->children_.size());
}

auto GRK_TransformComponent::GetWorldPosition() const -> glm::dvec3 {
  //if i have no parent, my position is world position
  //otherwise my position concatonated with my parents' world position (recursive) is my actual position
  if (parent_ == nullptr) {
    return localPosition_;
  } else {
    return localPosition_ + parent_->GetWorldPosition();
  }
}

auto GRK_TransformComponent::SetWorldPosition(glm::dvec3 v) -> void {
  //if no parent, setting world position is setting my position
  //otherwise it is these coordinates subtracted from my parent's position recursively
  if (parent_ == nullptr) {
    localPosition_ = v;
  } else {
    localPosition_ = v - parent_->GetWorldPosition();
  }
}

auto GRK_TransformComponent::SetWorldPosition(const double x, const double y, const double z) -> void {
  //if no parent, setting world position is setting my position
  //otherwise it is these coordinates subtracted from my parent's position recursively
  if (parent_ == nullptr) {
    localPosition_.x = x;
    localPosition_.y = y;
    localPosition_.z = z;
  } else {
    localPosition_.x = x - parent_->GetWorldPosition().x;
    localPosition_.y = y - parent_->GetWorldPosition().y;
    localPosition_.z = z - parent_->GetWorldPosition().z;
  }
}

auto GRK_TransformComponent::GetLocalPosition() const -> glm::dvec3 {
  return localPosition_;
}

auto GRK_TransformComponent::GetLocalPosition(glm::dvec3 v) -> void {
  this->localPosition_ = v;
}

auto GRK_TransformComponent::TranslateLocal(glm::dvec3 v) -> void {
  this->TranslateLocal(v.x, v.y, v.z);
}

auto GRK_TransformComponent::TranslateLocal(const double x, const double y, const double z) -> void {
  localPosition_.x += x;
  localPosition_.y += y;
  localPosition_.z += z;
}

auto GRK_TransformComponent::GetLocalScale() const -> glm::dvec3 {
  return localScale_;
}

auto GRK_TransformComponent::SetLocalScale(glm::dvec3 v) -> void {
  localScale_ = v;
}

auto GRK_TransformComponent::SetLocalScale(const double x, const double y, const double z) -> void {
  localScale_.x = x;
  localScale_.y = y;
  localScale_.z = z;
}

auto GRK_TransformComponent::DetachChildren() -> void {
  for (auto child : children_) {
    child->SetParent(nullptr);
  }
  children_.clear();
}

auto GRK_TransformComponent::GetChild(const unsigned int index) const -> GRK_TransformComponent* {
  return (index < children_.size() && index > 0) ? children_[index] : nullptr;
}

//template specialization for GRK_TransformComponent
//every entity MUST have this it cannot be destroyed
template<>
auto GRK_ComponentHandle<GRK_TransformComponent>::Destroy() const -> GRK_Result {
  //TODO 20 log that this cant happen
  return GRK_Result::Ok;
}

