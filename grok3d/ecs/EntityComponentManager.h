/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#ifndef __ENTITYCOMPONENENTMANAGER__H
#define __ENTITYCOMPONENENTMANAGER__H

#include "grok3d/grok3d_types.h"

#include "grok3d/ecs/entity/EntityHandle.h"

#include "grok3d/ecs/component/TransformComponent.h"
#include "grok3d/ecs/component/GameLogicComponent.h"
#include "grok3d/ecs/component/ComponentHandle.h"

#include "grok3d/ecs/system/SystemManager.h"

#include "notstd/bidir_map.h"
#include "notstd/tupleextensions.h"

#include <vector>
#include <tuple>
#include <unordered_map>
#include <functional>

namespace Grok3d {
/**
 * @brief The core class of game state storage and lifetime management
 *
 * @details
 * This class is at the heart of how the game engine fundamentally works.
 * At it's core, any game engine is a soft real time simulator and a simulator has two main
 * components (at least in my view):
 *     1. State -- all game engines have a state of the current scene, this includes all of the
 *     entities, their components, and those components individual state.  Essentially this is
 *     like you froze time and said "what does my game world look like right now"
 *     2. Update -- all game engines have rules for how that state will change over time.  This
 *     is what Systems and @link Grok3d::Systems::GRK_SystemManager GRK_SystemManager @endlink,
 *     the other core component of the game engine is responsible for.  The next state can be
 *     seen as a function of the current state plus some other factors.  These other factors are
 *     the systems they include things like:
 *          - User input
 *          - Gravity, Physics, and Collisions
 *          - Animations
 *          - Network State
 *          - Timed Events
 *          - AI and scripts
 *
 *  This class is largely responsible for managing the State aspect of simulating
 *
 * @tparam ComponentTypes This is a variadic template (thank you c++11!!!) list of all the types
 * of components that this class can manage.  The order in this list determines their unique
 * type ID as well as their bit in the @link
 * Grok3d::GRK_EntityComponentManager__::GRK_ComponentBitMask GRK_ComponentBitMask @endlink
 * for querying purposes*/
template<class... ComponentTypes>
class GRK_EntityComponentManager__ {
 private:
  using GRK_Entity = Grok3d::Entities::GRK_Entity;
  using GRK_EntityHandle = Grok3d::Entities::GRK_EntityHandle;
  using GRK_Component = Grok3d::Components::GRK_Component;
  template<class ComponentType>
  using GRK_ComponentHandle = Grok3d::Components::GRK_ComponentHandle<ComponentType>;
  using GRK_ComponentBitMask = Grok3d::Components::GRK_ComponentBitMask;
  using GRK_SystemManager = Grok3d::Systems::GRK_SystemManager;

  /**A tuple containing all the types in ComponentTypes*/
  using ComponentTuple = std::tuple<ComponentTypes...>;
  /**A tuple of vectors of each ComponentType**/
  using ComponentStoreTuple = std::tuple<std::vector<ComponentTypes>...>;

 public:
  GRK_EntityComponentManager__() noexcept :
      m_nextEntityId(1),
      m_deletedUncleanedEntities(std::vector<GRK_Entity>()),
      m_entityComponentsBitMaskMap(std::unordered_map<GRK_Entity, GRK_ComponentBitMask>(c_initial_entity_array_size)),
      m_entityComponentIndexMaps(std::vector<notstd::unordered_bidir_map<GRK_Entity, ComponentInstance>>()) {
    static_assert(notstd::ensure_parameter_pack_unique<ComponentTypes...>::value,
                  "The template arguments to GRK_EntityComponentManager__ must all be unique");

    m_deletedUncleanedEntities.reserve(c_initial_entity_array_size / 4);

    setup_component_stores(*this, m_componentStores);
  }

  /**
   * @brief Initializes the class by giving it a reference to the related systemManager to
   * register and unregister entities with each system
   *
   * @details
   * before any useful work can be done, this state manager class must have access to inform
   * the system half of the engine of important changes to state and register an entity with a
   * new system that is pertinent to it
   *
   * @param[in] systemManager the @link Grok3d::Systems::GRK_SystemManager GRK_SystemManager
   * @endlink that will manage updating the state
   *
   * @returns
   * @link Grok3d::GRK_Result::Ok Ok @endlink
   * @link Grok3d::GRK_Result::EntityAlreadyDeleted EntityAlreadyDeleted @endlink
   * @link Grok3d::GRK_Result::ComponentAlreadyAdded ComponentAlreadyAdded @endlink*/
  auto Initialize(GRK_SystemManager *systemManager) -> GRK_Result {
    m_systemManager = systemManager;
    m_isInitialized = true;

    return GRK_Result::Ok;
  }

  /**
   * @brief creates a unique entity and returns a handle to it
   *
   * @details
   * This handle will have a reference to this class in order to allow for manipulating the
   * entity's state and adding/removing of components.  From the point of view of the consumer
   * of the game engine, this class entirely encapsulates the concept of an entity and all
   * it's components
   *
   * @returns A handle to the newly created entity*/
  auto CreateEntity() -> GRK_EntityHandle {
    //I could do a check here to see if we overflowed to 0 but that's just inconceivable that we'd have that many (2^32) entities
    auto id = m_nextEntityId++;

    m_entityComponentsBitMaskMap[id] = 0;

    this->AddComponent(id, Grok3d::Components::GRK_TransformComponent());

    return GRK_EntityHandle(this, id);
  }

  /**
   * @brief Get the bitmask that describes all components that are a member of this entity
   *
   * @details
   * This bitmask is of type @link Grok3d::GRK_EntityComponentManager__::GRK_ComponentBitMask
   * GRK_ComponentBitMask @endlink and each set bit represents one ComponentType.
   *
   * @param[in] entity The entity you want the component information for
   *
   * @returns The bit mask of all the components the entity has*/
  auto GetEntityComponentsBitMask(const GRK_Entity entity) const -> GRK_ComponentBitMask {
    if (entity != 0) {
      return m_entityComponentsBitMaskMap.at(entity);
    } else {
      return 0;
    }
  }

  /**
   * @brief Deletes an entity
   *
   * @details
   * This adds an entity to a list of entities to be garbage collected at the end of the loop
   *
   * param[in] entity the entity to remove
   * @returns
   * @link Grok3d::GRK_Result::Ok Ok @endlink
   * @link Grok3d::GRK_Result::EntityAlreadyDeleted EntityAlreadyDeleted @endlink*/
  auto DeleteEntity(GRK_Entity entity) -> GRK_Result {
    if (entity == 0) {
      return GRK_Result::EntityAlreadyDeleted;
    }

    //ComponentManager's deleting their components is handled by GarbageCollection
    m_deletedUncleanedEntities.push_back(entity);

    // send a component bit mask of 0 to all Systems, no components means any system will unregister
    m_systemManager->UnregisterEntity(GRK_EntityHandle(this, entity));

    return GRK_Result::Ok;
  }

  /**A function that returns the vector of entities that are queued for GarbageCollection but not yet cleaned*/
  auto GetDeletedUncleanedEntities() -> std::vector<GRK_Entity> & {
    return m_deletedUncleanedEntities;
  }

  /**
   * @brief returns the position of the ComponentType parameter in this class's ComponentTypes
   * variadic template parameter
   *
   * @details
   * this is done using the related notstd library of template meta functions, specifically
   * with a call to notstd::type_to_index
   *
   * @tparam ComponentType The Component you want the index for*/
  template<class ComponentType>
  static constexpr auto GetComponentTypeAccessIndex() -> size_t {
    return notstd::type_to_index<ComponentType, ComponentTuple>::value;
  }

  /**
   * @brief Adds a component to the an entity in the scene
   *
   * @details
   * This finds the appropriate component storage vector and does the necessary housekeeping by
   *      -# Determining if the entity has that component (only one of each component type is
   *      allowed, it doesnt make sense to have two rigid bodies or two models does it)
   *      -# Updating the map of entities to indexes of the appropriate @link
   *      Grok3d::GRK_EntityComponentManager__::m_componentStores m_componentStores @endlink vector
   *      -# Adding the component to that vector
   *      -# Informing the systemmanager of the change so each system can begin acting on any
   *      entities that have the correct components as appropriate
   *
   *  @param[in] newComponent The component to be moved into the this class and added to the
   *  entity in the scene
   *
   *  @tparam ComponentType The type of component being added, leave it to compiler deduction
   *  from the argument
   *
   *  @returns
   *  @link Grok3d::GRK_Result::Ok Ok @endlink
   *  @link Grok3d::GRK_Result::EntityAlreadyDeleted EntityAlreadyDeleted @endlink
   *  @link Grok3d::GRK_Result::ComponentAlreadyAdded ComponentAlreadyAdded @endlink*/
  template<class ComponentType>
  auto AddComponent(GRK_Entity entity, ComponentType &&newComponent) -> GRK_Result {
    static_assert(notstd::param_pack_has_type<ComponentType, ComponentTypes...>::value,
                  "AddComponent Function requires ComponentType be one of the template params of GRK_EntityComponentManager__");

    if (entity == 0) {
      return GRK_Result::EntityAlreadyDeleted;
    }

    auto componentTypeIndex = GetComponentTypeAccessIndex<ComponentType>();

    //only add a component if one doesnt exist
    if ((m_entityComponentsBitMaskMap[entity] & IndexToMask(componentTypeIndex)) == 0) {
      // TODO should be ref but will be updated soon
      auto &componentTypeVector =
          std::get<std::vector<ComponentType>>(m_componentStores);

      if (componentTypeVector.size() == componentTypeVector.max_size()) {
        return GRK_Result::NoSpaceRemaining;
      } else {
        //TODO 8 have 2 CVARs that customizes load reallocation size, for now i'm doing 10% larger
        //SCALE NUM and SCALE DEN
        //DOCUMENTATION put the formula for scale factor of this in the docs, %scale = (1+NUM/DEN)

        //resize vector if necessary
        const auto cap = componentTypeVector.capacity();
        if (cap == componentTypeVector.size()) {
          componentTypeVector.reserve(cap + cap / 10);
        }

        //add the component to the end our vector
        componentTypeVector.push_back(std::move(newComponent));

        auto &entityInstanceMap = m_entityComponentIndexMaps.at(componentTypeIndex);

        //the new size - 1 is the index of the vector the element is stored at
        entityInstanceMap.put(entity, static_cast<ComponentInstance>(componentTypeVector.size() - 1));

        m_entityComponentsBitMaskMap[entity] |=
            IndexToMask(componentTypeIndex);

        //inform all systems of new component added to this entity
        m_systemManager->UpdateSystemEntities(GRK_EntityHandle(this, entity));

        return GRK_Result::Ok;
      }
    } else {
      return GRK_Result::ComponentAlreadyAdded;
    }
  }

  /**
   * @brief Get a handle to a component for a given entity
   *
   * @details
   * This looks through the internal data stores and gives you back a handle (which includes a
   * pointer) to the component for use in systems
   *
   * @param[in] entity the entity you would like to query about
   *
   * @tparam ComponentType the type of component you'd like to get a handle for*/
  template<class ComponentType>
  auto GetComponent(GRK_Entity entity) const -> GRK_ComponentHandle<ComponentType> {
    static_assert(notstd::param_pack_has_type<ComponentType, ComponentTypes...>::value,
                  "GetComponent Function requires ComponentType be one of the template params of GRK_EntityComponentManager__");

    auto componentTypeIndex = GetComponentTypeAccessIndex<ComponentType>();

    const GRK_ComponentBitMask componentMask =
        static_cast<GRK_ComponentBitMask>(IndexToMask(componentTypeIndex));

    if (entity == 0 || (m_entityComponentsBitMaskMap.at(entity) & componentMask) == 0) {
      return GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
    } else if ((m_entityComponentsBitMaskMap.at(entity) & componentMask) == componentMask) {
      //this is a vector of the type we are trying to remove
      // TODO should be ref but will be changed soon
      const auto &componentTypeVector =
          std::get<std::vector<ComponentType>>(m_componentStores);

      const auto &entityInstanceMap = m_entityComponentIndexMaps.at(componentTypeIndex);

      //get the instance (index in our vector) from teh entityInstanceMap
      const auto instance = entityInstanceMap.at(entity);

      //use the instance to index the array of that componenttype
      const auto *const componentPointer = &(componentTypeVector.at(instance));

      //return it in a handle
      return GRK_ComponentHandle<ComponentType>(this, componentPointer, entity);
    } else {
      return GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
    }
  }

  /**
   * @brief get the entire component store vector reference
   *
   * @details
   * this is useful in systems like rendersystem that need to go through all of a certain
   * component type
   *
   * @tparam ComponentType the type of component store vector you'd like to get a refernce of*/
  template<class ComponentType>
  auto GetComponentStore() const -> const std::vector<ComponentType> * {
    return &std::get<std::vector<ComponentType>>(m_componentStores);
  }

  /**
   * @brief remove a component from an entity
   *
   * @details
   * Works similarly to the AddComponent function, except in reverse (hah)
   * it is done via the following procedure:
   *      -# Check if the entity and ComponentType are valid and get the component store.
   *      -# use the entity to store index map to get the index to remove
   *      -# Move the last element in the list into that index, or just clear if it is size 1
   *      -# Update the map to reflect that information for the moved Component
   *      -# pop_back the storage vector to shrink the size by 1
   *
   *  @param[in] entity the entity you are removing from
   *
   *  @tparam ComponentType The type of component to remove from the entity (there is only
   *  ever one of each type)*/
  template<class ComponentType>
  auto RemoveComponent(GRK_Entity entity) -> GRK_Result {
    static_assert(notstd::param_pack_has_type<ComponentType, ComponentTypes...>::value,
                  "RemoveComponent Function requires ComponentType be one of the template params of GRK_EntityComponentManager__");

    const auto componentMask = static_cast<GRK_ComponentBitMask>(IndexToMask(
        GetComponentTypeAccessIndex<ComponentType>()));

    if (entity == 0) {
      return GRK_Result::EntityAlreadyDeleted;
    }

    if ((m_entityComponentsBitMaskMap[entity] & componentMask) > 0) {
      //TODO GC this as well, when you remove component you should just remove it from
      //entity mask and store the actual component from the store's ComponentInstance in a
      //m_deletedUncleanedComponents vector to iterate through
      //there may be some overlap with the entity removal pass so use .find(entity) and
      //check to be sure it is already out of the map
      return RemoveComponentHelper<ComponentType>(entity);
    } else {
      return GRK_Result::NoSuchElement;
    }
  }

  // Garbage collects deleted entities (this is Components are always directly deleted as of now)
  auto GarbageCollect() -> void {
    //TODO dont always do this lets be smarter
    garbage_collect_iter();
  }

 private:
  /**This function does most of the nitty gritty work of @link
   * Grok3d::GRK_EntityComponentManager__::RemoveComponent RemoveComponent but is broken out
   * for convenience and reuse in other scenarios such as GarbageCollection*/
  template<class ComponentType>
  auto RemoveComponentHelper(GRK_Entity entity) -> GRK_Result {
    static_assert(notstd::param_pack_has_type<ComponentType, ComponentTypes...>::value,
                  "RemoveComponentHelper Function requires ComponentType be one of the template params of GRK_EntityComponentManager__");

    const auto componentAccessIndex = GetComponentTypeAccessIndex<ComponentType>();
    //this is a vector of the type we are trying to remove
    auto &componentTypeVector = std::get<std::vector<ComponentType>>(m_componentStores);

    //this is the map of entity to components for this type
    auto &entityInstanceMap = m_entityComponentIndexMaps.at(componentAccessIndex);

    //check if the elment is in the map
    //and do what we need to if it is not
    if (entityInstanceMap.find(entity) == entityInstanceMap.end()) {
      return GRK_Result::NoSuchElement;
    } else {
      //this entity exists so we move the last element of the components vector
      //to the spot that this one was taking up

      // Index of element we are removing
      const auto removeIndex = entityInstanceMap.at(entity);

      // Instance of element we are moving
      auto lastElementEntity = entityInstanceMap.reverse_at(componentTypeVector.size() - 1);

      // if element is not last then we move last into this one's place
      if (lastElementEntity != entity) {
        // Element we are moving into it's old place
        auto &lastElement = componentTypeVector.back();

        //use std::move so we cannibilize any allocated components and dont copy them
        componentTypeVector[removeIndex] = std::move(lastElement);
      }

      //then remove it from the map
      //and shorten our vector
      entityInstanceMap.erase(entity);
      componentTypeVector.pop_back();

      // If there is nothing in componentTypeVector we just erased the last element, so no need to update map
      if (componentTypeVector.size() > 0) {
        entityInstanceMap.reverse_erase(componentTypeVector.size());
        entityInstanceMap.put(lastElementEntity, removeIndex);
      }

      //remove it from bitmask
      m_entityComponentsBitMaskMap[entity] &= ~(IndexToMask(componentAccessIndex));

      return GRK_Result::Ok;
    }
  }

 private:
  /** @fn
   * @brief Meta function which sets up component store vectors stored in the
   * @link Grok3d::GRK_EntityComponentManager__::m_componentStores m_componentStores tuple
   * @endlink
   *
   * @details
   * This is done by going through each index in the tuple and reserving some space in the
   * vectors to prevent long load times later and initializing the map of entities to indexes in
   * the stores
   *
   * @tparam index the index in the tuple to intialize
   * @tparam Ts variadic list of types that are the types in the tuple to be initialized*/
  template<int index, class... Ts>
  struct setup_component_stores_impl {
    /**
     * the applictive operator for the meta function
     *
     * @param ecm refernce to this
     * @param t the tuple member of ecm being initialized*/
    auto operator()(GRK_EntityComponentManager &ecm, std::tuple<Ts...> &t) -> void {
      auto &elem = std::get<index>(t);
      elem.reserve(c_initial_entity_array_size);
      ecm.m_entityComponentIndexMaps.push_back(
          notstd::unordered_bidir_map<GRK_Entity, ComponentInstance>(c_initial_entity_array_size));
      setup_component_stores_impl<index - 1, Ts...>{}(ecm, t);
    }
  };

  /**@overload*/
  template<class... Ts>
  struct setup_component_stores_impl<-1, Ts...> {
    auto operator()(GRK_EntityComponentManager &ecm, std::tuple<Ts...> &t) -> void {}
  };

  /**Meta convenience function to call the setup_component_stores meta function
   *
   * @param ecm this class
   * @param t the tuple that is being iterated through and set up
   *
   * @tparam Ts deduced from the tuple that is passed to this function*/
  template<class... Ts>
  auto setup_component_stores(GRK_EntityComponentManager &ecm, std::tuple<Ts...> &t) -> void {
    const auto size = sizeof...(Ts);
    setup_component_stores_impl<size - 1, Ts...>{}(ecm, t);
  }

  /** @fn
   * @brief Meta function for garbage collection iteration
   *
   * @details
   * This function iterates through each of the types of Components and checks if that
   * component type is contained in a deleted entity that is being garbage collected, if so it
   * runs the @link Grok3d::GRK_EntityComponentManager__::RemoveComponentHelper
   * RemoveComponentHelper @endlink function on it
   *
   * @tparam ComponentIndex the index in Ts we are checking for removal
   * @tparam Ts ComponentTypes passed to the meta function
   * */
  template<int ComponentIndex, class... Ts>
  struct garbage_collect_iter_impl {
    /** The applicitive operator of the meta function*/
    auto operator()(GRK_EntityComponentManager &ecm) -> void {
      for (auto &entity : ecm.m_deletedUncleanedEntities) {
        using ComponentType = typename notstd::index_to_type<ComponentIndex, Ts...>::type;
        if ((ecm.m_entityComponentsBitMaskMap[entity] & IndexToMask(ComponentIndex)) > 0) {
          ecm.template RemoveComponentHelper<ComponentType>(entity);
        }
      }

      garbage_collect_iter_impl<ComponentIndex - 1, Ts...>{}(ecm);
    }
  };

  /**@overload*/
  template<class... Ts>
  struct garbage_collect_iter_impl<-1, Ts...> {
    auto operator()(GRK_EntityComponentManager &ecm) -> void {}
  };

  /**Meta convenience function to call the garbage_collect_iter_impl meta function*/
  auto garbage_collect_iter() -> void {
    const auto size = sizeof...(ComponentTypes);
    garbage_collect_iter_impl<size - 1, ComponentTypes...>{}(*this);
    m_deletedUncleanedEntities.clear();
  }

 private:
  bool m_isInitialized = false;                       ///< Simple bool t be sure we have a valid m_systemManager

  GRK_SystemManager
      *m_systemManager;                 ///< The system manager that handles updating the state stored here

  ComponentStoreTuple m_componentStores;              ///< The tuple of vectors which store each component type

  GRK_Entity
      m_nextEntityId;                          ///< The next entity ID that is incremened each time an entity is made

  std::vector<GRK_Entity> m_deletedUncleanedEntities; ///< list of deleted entites that need to be Garbage Collected

  typedef size_t ComponentInstance;                   ///< index into vector for component

  ///this is a map of entities to a bitmask of their components, used for system registration/component deletion checks etc
  std::unordered_map<GRK_Entity, GRK_ComponentBitMask> m_entityComponentsBitMaskMap;
  ///vector of bidirectional maps from entity to component index into std::get<ComponetIndex>(m_componentStores)[]
  mutable std::vector<notstd::unordered_bidir_map<GRK_Entity, ComponentInstance>> m_entityComponentIndexMaps;
};
} /*Grok3d*/

#endif
