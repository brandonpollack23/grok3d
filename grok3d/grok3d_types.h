/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file
 * @brief The mother file of type definitions
 * @details
 * this file contains all of the typedefs, forward type declarations, enums, general helper
 * functions (having to do with types) and operators for defined types
 */
#ifndef __GROK3d_TYPES__H
#define __GROK3d_TYPES__H

#include "grok3d/glad/glad/glad.h"

#include <type_traits>
#include <functional>

/**constant (for now, future to make CVAR) that controls initial size of stores*/
constexpr auto c_initial_entity_array_size = 1024;

//configure GLM
#define GLM_PRECISION_HIGHP_FLOAT

/**converts a number to that bit set from least significant bit, for constructing bitmasks*/
constexpr unsigned int IndexToMask(std::size_t index) {
  return 1 << index;
}

/** The namespace for the project*/
namespace Grok3d {
class GRK_Engine;

/**all the possible result statuses*/
enum class GRK_Result : std::size_t {
  Ok = 0,                                ///< No problems
  NoSuchElement = 1,                     ///< Returned when there is no Component or entity queried about
  NoSpaceRemaining = 1 << 1,             ///< Uh oh, no more space, how did you even manage?
  NoSuchEntity = 1 << 2,                 ///< Returned when that entity does not exist (anymore)
  EntityAlreadyDeleted = 1 << 3,         ///< Returned when it is certain the entity existed but is now gone
  EntityAlreadyHasComponent = 1 << 4,    ///< Only one of each component on an entity allowed
  ComponentAlreadyAdded = 1 << 6,        ///< Returned when entity has already added this component type
  ErrorAddingToSystem = 1 << 9,          ///< returned when there is some sort of system registration error
  EngineFailureNoInitialState = 1 << 10, ///< Engine must be initialized before use
  CriticalError = 1 << 11,               ///< Wow...I blame you >_>
  RenderingTerminated = 1 << 12,         ///< Rendering is done
  OpenGLErrorOccurred = 1 << 13          ///< Some OpenGL error happened, check std::err
};

using UT_GRK_Result = std::underlying_type_t<GRK_Result>;

inline auto operator|(GRK_Result lhs, GRK_Result rhs) -> GRK_Result {
  return (GRK_Result) (static_cast<UT_GRK_Result>(lhs) | static_cast<UT_GRK_Result>(rhs));
}

inline auto operator|=(GRK_Result &lhs, GRK_Result rhs) -> GRK_Result & {
  lhs = (GRK_Result) (static_cast<UT_GRK_Result>(lhs) | static_cast<UT_GRK_Result>(rhs));
  return lhs;
}

enum class GRK_GL_PrimitiveType : GLenum {
  Unsigned_Int = GL_UNSIGNED_INT,
  Unsigned_Byte = GL_UNSIGNED_BYTE,
  Unsigned_Short = GL_UNSIGNED_SHORT
};

/** The namespace which contains all Entity related code*/
namespace Entities {
/**
 * @brief A unique ID that identifies an object in the game world
 */
typedef std::size_t GRK_Entity;
}

/** The namespace which contains all Compoenents related code and Compoenents definitions*/
namespace Components {
class GRK_Component;

class GRK_TransformComponent;

class GRK_GameLogicComponent;

class GRK_GameBehaviourBase;

class GRK_RenderComponent;

using GRK_VertexBufferObject  = unsigned int;
using GRK_VertexArrayObject   = unsigned int;
using GRK_ElementBufferObject = unsigned int;

typedef int GRK_ComponentBitMask;
}

/** The namespace which contains all System related code and System definitions*/
namespace Systems {
class GRK_SystemManager;

class GRK_System;

class GRK_GameLogicSystem;
}

/**
 * @brief The specialized version of @link Grok3d::GRK_EntityComponentManager__ GRK_EntityComponentManager__ @endlink that is used throughout the
 * engine, it contains all of the types of components supported in a paramater pack
 * This is where you add your components to the engine, by adding to this template argument list
 *
 * @see GRK_EntityComponentManager__
 */
template<class... ComponentTypes>
class GRK_EntityComponentManager__;

using GRK_EntityComponentManager = GRK_EntityComponentManager__<Components::GRK_TransformComponent,
                                                                Components::GRK_GameLogicComponent,
                                                                Components::GRK_RenderComponent>;

namespace Components {
template<class ComponentType, class ECM = GRK_EntityComponentManager>
class GRK_ComponentHandle;
}
namespace Entities {
/**Specialized version of
 * @link Grok3d::Entities::GRK_EntityHandle__ GRK_EntityHandle__ @endlink with
 * the specialized version of @link Grok3d::GRK_EntityComponentManager__ GRK_EntityComponentManager__ @endlink*/
template<class ECM = GRK_EntityComponentManager>
class GRK_EntityHandle__;

using GRK_EntityHandle = GRK_EntityHandle__<GRK_EntityComponentManager>;
}

/** Namespace that contains various other classes and helpers that are not necessarily part of
 * the ECS*/
namespace Utilities {
class GRK_ShaderManager;
}
}

/**std namespace, added a hash function*/
namespace std {
/*GRK_EntityHandle*/
/*hash function for GRK_EntityHandle, just uses the entity hash*/
template<class ECM>
struct hash<Grok3d::Entities::GRK_EntityHandle__<ECM>> {
  typedef Grok3d::Entities::GRK_EntityHandle__<ECM> argument_type;
  typedef size_t result_type;

  auto operator()(argument_type const &e) const -> result_type;
};
}

#endif
