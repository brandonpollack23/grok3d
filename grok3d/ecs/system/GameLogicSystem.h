/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#ifndef __GAMELOGICSYSTEM__H
#define __GAMELOGICSYSTEM__H

#include "grok3d/grok3d_types.h"
#include "grok3d/ecs/system/System.h"

namespace Grok3d {
/**
 * @brief the game logic system which handles updating of scripts
 *
 * @details This system oversees the iteration and updating of all the behaviours on all entites
 * with a @link Grok3d::GRK_GameLogicComponent GRK_GameLogicComponent @endlink*/
class GRK_GameLogicSystem : public GRK_System {
 public:
  GRK_GameLogicSystem() noexcept;

  /**Overrided function that implements the iteration over all the tracked
   * entities and running update on their
   * @link Grok3d::GRK_GameLogicComponent GRK_GameLogicComponent @endlink*/
  auto UpdateInternal(double dt) -> GRK_Result override;

 protected:
  /**@see @link Grok3d::GRK_System::GetComponentsBitMask
   * GRK_System::GetComponentsBitMask @endlink*/
  auto GetComponentsBitMask() const -> Grok3d::GRK_ComponentBitMask override;
};
}

#endif
