/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#ifndef __SYSTEM__H
#define __SYSTEM__H

#include "grok3d/grok3d_types.h"
#include "grok3d/ecs/entity/EntityHandle.h"

#include <unordered_set>

namespace Grok3d {
/**
 * @brief Base class for a system
 *
 * @details
 * this is one of the few cases where I am using dynamic dispatch
 * so that i can easily call the update function on them all as well as have them have the
 * potential to be implemented in other languages in the future
 */
class GRK_System {
 public:
  GRK_System() noexcept;

  /**
   * @brief Runs @link Grok3d::GRK_System::UpdateInternal UpdateInternal @endlink
   * function as well as other houskeeping
   *
   * @details
   * This is seperate because an implementor of a system need only be concerned with the
   * update function, no need to worry about cleaning up and unregistering entities because of
   * nuances in iterating through vectors etc.
   *
   * @parma[in] dt the amount of time to simulate
   *
   * @results can be any errors in removal of componetns or any errors that UpdateInternal
   * decides to throw out*/
  GRK_Result Update(double dt);

  /**the function to be overrided to implement the system behaviour*/
  virtual GRK_Result UpdateInternal(double dt) = 0;

  /**Function to register an entity based on if it's owned components contain at least
   * everything in @link Grok3d::GRK_System::GetComponentsBitMask
   * GetComponentsBitMask @endlink*/
  auto UpdateSystemEntities(const Grok3d::GRK_EntityHandle &entity) -> Grok3d::GRK_Result;

  /**Queue an entity to be removed from the system's update queue*/
  auto UnregisterEntity(const Grok3d::GRK_EntityHandle &entity) -> Grok3d::GRK_Result;

 protected:
  /**Virtual function to override for each system that returns a mask of all the ComponetTypes
   * they are concerned with*/
  virtual auto GetComponentsBitMask() const -> Grok3d::GRK_ComponentBitMask = 0;

 private:
  /**Seperate function that completes unregistration since we cant modify a iterable
   * while it's being iterated through*/
  auto CompleteUnregisterEntities() -> Grok3d::GRK_Result;

 protected:
  std::unordered_set<Grok3d::GRK_EntityHandle> m_trackedEntities; ///< entities this system will update
  std::vector<Grok3d::GRK_EntityHandle>
      m_entitiesToUnregister;   ///< entities that will be removed after updating
};
}

#endif
