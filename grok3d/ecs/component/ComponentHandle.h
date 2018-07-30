/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#ifndef __COMPONENTHANDLE__H
#define __COMPONENTHANDLE__H

#include "grok3d/grok3d_types.h"

namespace Grok3d {
/**
 *  @brief A handle to a Component for convenient management
 *
 *  @details
 *  This class template stores a reference to the creating @link GRK_EntityComponentManager
 *  GRK_EntityComponentManager @endlink and makes management of the lifetime,
 *  getting related properties, and usage of a component safer and easier to read
 *
 *  @tparam ComponentType The type of the component contained by the specialization
 *  @tparam ECM The type of the GRK_EntityComponentManager__ who created the handle, this can
 *  change depending on what components are in the template arguments list for it*/
template<class ComponentType, class ECM>
class GRK_ComponentHandle {
 public:
  /**
   * @param[in] entityComponentManager The manager which created this handle, passed in as
   * "this" on construction
   * @param[in] component The raw component pointer, points directly to the component's
   * location in the GRK_EntityComponentManager__::componentStores_ tuple of vectors
   * @param[in] owner The @link GRK_Entity GRK_Entity @endlink to which this GRK_Component* will belong*/
  GRK_ComponentHandle(
      const ECM* entityComponentManager,
      const ComponentType* component,
      const GRK_Entity owner) noexcept :
      owner_(owner),
      component_(component),
      manager_(entityComponentManager) {
  }

  /**Returns the owning entity member*/
  auto GetOwningEntity() const -> const GRK_Entity {
    return owner_;
  }

  /**Uses ECM to find the Variadic index of this ComponentType, and affirms that the owning
   * entity has a component of this type*/
  auto IsHandleValid() const -> bool {
    auto thisComponentBitMask = IndexToMask(ECM::template GetComponentTypeAccessIndex<ComponentType>());
    auto components = manager_->GetEntityComponentsBitMask(owner_);
    return ((components & thisComponentBitMask) == thisComponentBitMask);
  }

  /**dereferences and fowards to internal ComponentType*/
  auto operator->() -> ComponentType* {
    if (IsHandleValid()) {
      return const_cast<ComponentType*>(component_);
    } else {
      return nullptr;
    }
  }

  /**forwards a call to the manager to remove this component from owning entity*/
  auto Destroy() const -> GRK_Result {
    return manager_->template RemoveComponent<ComponentType>(owner_);
  }

 private:
  /// The @link GRK_Entity GRK_Entity @endlink to which this GRK_Component* will belong
  const GRK_Entity owner_;

  /** @brief The raw component pointer, points directly to the component's location in the
   * @link GRK_EntityComponentManager__::componentStores_ GRK_EntityComponentManager__::componentStores_ @endlink
   * tuple of vectors*/
  const ComponentType* component_;

  /// The manager which created this handle, passed in as "this" on construction
  const ECM * manager_;
};
}

#endif
