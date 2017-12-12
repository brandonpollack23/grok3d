#ifndef __MOVEBACKANDFORTHTEST__H
#define __MOVEBACKANDFORTHTEST__H

#include "grok3d.h"

#include "../inc/movebackandforthtest.h"

auto CreateAndRegisterMoveTestEntity(Grok3d::GRK_EntityComponentManager& ecm) -> Grok3d::GRK_Result;

auto CreationMovementDeleteTest() -> void;

#endif
