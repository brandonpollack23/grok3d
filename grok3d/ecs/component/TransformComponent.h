/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file
 * Definition for transform componenent*/
#ifndef __TRANSLATIONCOMPONENT__H
#define __TRANSLATIONCOMPONENT__H

#include "grok3d/grok3d_types.h"

#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"

#include <string>
#include <vector>

namespace Grok3d {
/**
 * @brief The component that all entities have that determines their position in the game world.
 *
 * @details
 * All entities are required to have a GRK_TransformComponent.  It is
 * created for an entity when the entity is created, and removing it is not allowed.
 * Semantically, it doesn't make sense to not have a position in a world, and you should not be
 * a game entity if you do not.  There will be other means to run worldwide scripts and keep
 * track of worldwide state in the future.
 *
 * Each TransformComponent has a parent they are relative to to facilitate chaining and relative
 * position, or creation of complex objects composed of multiple entities (such as a giant boss
 * with independent rotating turrets...or a boring old tank with a rotating cannon)*/
class GRK_TransformComponent {
 public:
  GRK_TransformComponent() noexcept;

  //Functions related to children and other relatives

  //TODO as chaining lots of parents gets really big this will get slow, best to cache and have a dirty bit
  /**
   * @brief Attaches this TransformComponent to a new parent to be relative to.
   * @details
   * calls @link GRK_TransformComponent::AttachChild AttachChild @endlink
   * on the new parent
   *
   * @param[in] newParent the parent that AttachChild will be called on*/
  auto SetParent(GRK_TransformComponent* newParent) -> void;

  /**
   * @brief Attaches a child to this parent
   *
   * @param[in] newChild the child that will become relative to this parent*/
  auto AttachChild(GRK_TransformComponent* newChild) -> void;

  /**
   * @brief tests if this class is the child of another TransformComponent
   *
   * param[in] possibleParent The TransformComponent to test if we are the child of*/
  auto IsChildOf(const GRK_TransformComponent* const possibleParent) const -> bool;

  /**Find the index you are in the parents pool of children*/
  auto GetSiblingIndex() -> std::size_t; //use parent and iterate through the list looking for me

  /**
   * @brief get the index in your pool of children for the child
   * @param[in] possibleChild the child your are indexing, possible that it is not a child
   * at all
   * @return the index of the child from 0, -1 if it is not a child*/
  auto GetChildIndex(const GRK_TransformComponent* possibleChild) const -> std::size_t;

  /**Returns number of children who are positioned relative to this*/
  auto ChildCount() -> int;

  //Functions related to position

  /**Get the position relative to the origin of the scene*/
  auto GetWorldPosition() const -> glm::dvec3;

  /**Set the position relative to the origin of the scene*/
  auto SetWorldPosition(glm::dvec3 v) -> void;

  /**@overload*/
  auto SetWorldPosition(double x, double y, double z) -> void;

  /**Get the position relative to the parent*/
  auto GetLocalPosition() const -> glm::dvec3;

  /**@overload*/
  auto GetLocalPosition(glm::dvec3 v) -> void;

  /**Change position relative to current position*/
  auto TranslateLocal(glm::dvec3 v) -> void;

  /**@overload*/
  auto TranslateLocal(double x, double y, double z) -> void;

  //Functions related to scale

  /**Get scale of object relative to parent*/
  auto GetLocalScale() const -> glm::dvec3;

  /**Set scale of object relative to parent*/
  auto SetLocalScale(glm::dvec3 v) -> void;

  /**@overload*/
  auto SetLocalScale(double x, double y, double z) -> void;

  //functionality

  /**Detach all of the children from this parent*/
  auto DetachChildren() -> void;

  /**Get a child by index*/
  auto GetChild(unsigned int index) const -> GRK_TransformComponent*;

 private:
  /// The parent of this TransformComponent.
  GRK_TransformComponent* parent_;

  /// The vector of children who are positioned relative to this TransformComponent.
  std::vector<GRK_TransformComponent*> children_;

  /// Position relative to parent TransformComponent.
  glm::dvec3 localPosition_;

  /// Scale relative to parent TransformComponent.
  glm::dvec3 localScale_;

  //TODO rotation as quaternion
};
}

#endif
