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
   * location in the GRK_EntityComponentManager__::m_componentStores tuple of vectors
   * @param[in] owner The @link GRK_Entity GRK_Entity @endlink to which this GRK_Component* will belong*/
  GRK_ComponentHandle(
      const ECM* entityComponentManager,
      const ComponentType* component,
      const GRK_Entity owner) noexcept :
      m_owner(owner),
      m_component(component),
      m_manager(entityComponentManager) {
  }

  /**Returns the owning entity member*/
  auto GetOwningEntity() const -> const GRK_Entity {
    return m_owner;
  }

  /**Uses ECM to find the Variadic index of this ComponentType, and affirms that the owning
   * entity has a component of this type*/
  auto IsHandleValid() const -> bool {
    auto thisComponentBitMask = IndexToMask(ECM::template GetComponentTypeAccessIndex<ComponentType>());
    auto components = m_manager->GetEntityComponentsBitMask(m_owner);
    return ((components & thisComponentBitMask) == thisComponentBitMask);
  }

  /**dereferences and fowards to internal ComponentType*/
  auto operator->() -> ComponentType* {
    if (IsHandleValid()) {
      return const_cast<ComponentType*>(m_component);
    } else {
      return nullptr;
    }
  }

  /**forwards a call to the manager to remove this component from owning entity*/
  auto Destroy() const -> GRK_Result {
    return m_manager->template RemoveComponent<ComponentType>(m_owner);
  }

 private:
  const GRK_Entity
      m_owner; ///< The @link GRK_Entity GRK_Entity @endlink to which this GRK_Component* will belong
  const ComponentType* m_component;           /**< @brief The raw component pointer, points directly
                                                         to the component's location in the 
                                                         @link GRK_EntityComponentManager__::m_componentStores
                                                         GRK_EntityComponentManager__::m_componentStores @endlink tuple of vectors*/
  const ECM
      * m_manager;                       ///< The manager which created this handle, passed in as "this" on construction
};
}

#endif
