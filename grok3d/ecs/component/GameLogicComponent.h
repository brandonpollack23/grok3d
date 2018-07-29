/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file
 * contains definitions for both @link Grok3d::GRK_GameLogicComponent
 * GRK_GameLogicComponent @endlink and @link Grok3d::GRK_GameBehaviourBase
 * GRK_GameBehaviourBase @endlink*/

#ifndef __GAMELOGICCOMPONENT__H
#define __GAMELOGICCOMPONENT__H

#include "grok3d/grok3d_types.h"

#include "grok3d/ecs/EntityComponentManager.h"
#include "grok3d/ecs/entity/EntityHandle.h"

#include "notstd/bidir_map.h"

#include <vector>
#include <memory>

namespace Grok3d {
/**
 * @brief
 * The component that controls execution of game level scripts and behaviours
 *
 * @details
 * This class acts as a data container for a list of unique "behaviours" which act as the
 * implementation of game level logic.  These are classes act as a specialized functor by
 * extending @link Grok3d::GRK_GameBehaviourBase GRK_GameBehaviourBase @endlink.*/
class GRK_GameLogicComponent {
 public:
  /**A handle to a behaviour, it is an integer that works much like a file descriptor, it
   * is your key into the @link Grok3d::GRK_GameLogicComponent
   * GRK_GameLogicComponent @endlink to access your behaviour, usually for removal*/
  typedef size_t BehaviourHandle;

 public:
  GRK_GameLogicComponent() noexcept;

  GRK_GameLogicComponent(GRK_GameLogicComponent &&glc) noexcept;

  GRK_GameLogicComponent &operator=(GRK_GameLogicComponent &&rhs) noexcept;

  /**
   * @brief Update function which takes a change in time since the last update, this then calls all the behaviours with
   * the same change in time
   *
   * @param[in] dt the change in time since the last update*/
  auto Update(double dt) -> void;

  /**
   * @brief Adds a behaviour to the container and returns a handle for removal later
   *
   * @details
   * once a behaviour is registered, it is moved completely inside this object, and can
   * only be interacted with via the returned BehaviourHandle.  Naturally, this object now
   * "owns" it and manages its lifetime as well
   *
   * @param[in] behaviour the behaviour that is being copied into this struct, lifetime is
   * now managed by this object*/
  auto RegisterBehaviour(std::unique_ptr<GRK_GameBehaviourBase> behaviour) -> BehaviourHandle;

  /**
   * @brief This function sets up a behaviour to be removed*
   *
   * @details
   * it does not remove it right away, because the nature of component updates is that you
   * are iterating through all of them.  It is ill advised to modify a container of these
   * behaviours while you are iterating through it, and will break the program, so instead
   * we keep track of the fact that we are going to remove the behaviour and when we are
   * done with iterating through them, we update the container and clean up.
   *
   * @param[in] handle a token for modifying and removing later*/
  auto EnqueueBehaviourRemoval(BehaviourHandle handle) -> Grok3d::GRK_Result;

 protected:
  /**
   * @brief this does the actual work of removing a behaviour
   *
   * @details
   * after a behaviour has ::EnqueueBehaviourRemoval called for it, this is the function
   * which runs later and actually frees the memory and takes it out of the queue
   *
   * @param[in] handle the behaviour handle to be removed*/
  auto UnregisterBehaviour(BehaviourHandle handle) -> Grok3d::GRK_Result;

 protected:
  typedef size_t BehaviourIndex; ///< the index into m_behaviours that corresponds to a behaviour

  static BehaviourHandle s_nextHandle; ///< a static counter that makes every BehaviourHandle unique (enough)

  notstd::unordered_bidir_map<BehaviourHandle, BehaviourIndex>
      m_behaviourIndexMap; ///< an internal bidirectional mapping of handles to indexes into m_behaviours
  std::vector<std::unique_ptr<GRK_GameBehaviourBase>>
      m_behaviours; ///< the actual store of behaviours, stored as unique_ptr for lifetime management

  std::vector<BehaviourHandle> m_behavioursToRemove; ///< the queue of behaviours to remove after done updating
};

/**
 * @brief This is the abstract base class which you extend to implement game level logic
 *
 * @details
 * this is one of the only classes heierarchies that uses dynamic dispatch (or heirarchies at all for that matter)
 * so that it can be overloaded and classes can be made in other languages later on (LUA or python)*/
class GRK_GameBehaviourBase {
 public:
  /**
   * @param[in] owningEntity often enough every single gamelogic component needs to add/remove/modify
   * components, they can do that with this @link Grok3d::GRK_EntityHandle
   * GRK_EntityHandle @endlink*/
  GRK_GameBehaviourBase(Grok3d::GRK_EntityHandle owningEntity) noexcept;

  /**
   * @brief function you override that updates the owning entity with included behaviour
   *
   * @details
   * this function is overrided and does all the side effects to change the state of your game
   * things you can do include: add components, remove them, modify them (position for
   * example).  The extended classes can also have any other members which an be accessed from
   * the overrides of this funcion.  In essense, everything in the world you need to access
   * should be in the extended class and accessed from this function.
   *
   * @param[in] dt time since last update, time to simulate*/
  virtual void Update(double dt) = 0;

 protected:
  /**Convenience function that calls @link Grok3d::GRK_GameLogicComponent::EnqueueBehaviourRemoval
   * EnqueueBehaviourRemoval @endlink for you*/
  Grok3d::GRK_Result UnregisterThisBehaviour();

 protected:
  friend class GRK_GameLogicComponent;

  Grok3d::GRK_EntityHandle m_owningEntity;         ///< the entity that this behaviour applies to

  GRK_GameLogicComponent::BehaviourHandle m_behaviourHandle; /**<
                                                                    * the handle to the behaviour,
                                                                    * used by @link Grok3d::GRK_GameBehaviourBase::UnregisterThisBehaviour
                                                                    * UnregisterThisBehaviour @endlink*/
};
}

#endif
