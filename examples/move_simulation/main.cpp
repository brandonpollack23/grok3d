/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

#include "grok3d/grok3d.h"

#include "movebackandforthbehaviour.h"

#include <iostream>

using namespace Grok3d;

auto CreateAndRegisterMoveTestEntity(GRK_EntityComponentManager &ecm) -> GRK_Result {
  auto pointEntity = ecm.CreateEntity();

  auto moveBehaviour = std::make_unique<MoveBackAndForthBehaviour>(pointEntity);

  GRK_GameLogicComponent glc;
  glc.RegisterBehaviour(std::move(moveBehaviour));

  return pointEntity.AddComponent(std::move(glc));
}

auto CreationMovementDeleteTest() -> void {
  auto engineInitialization = [](GRK_EntityComponentManager &ecm) -> GRK_Result {
    for (int i = 0; i < 5; i++) {
      auto result = CreateAndRegisterMoveTestEntity(ecm);
      if (result != GRK_Result::Ok) {
        return result;
      }
    }

    return GRK_Result::Ok;
  };

  GRK_Engine engine(engineInitialization);
  engine.Run();
}

auto main(int argc, char *argv[]) -> int {
  CreationMovementDeleteTest();
  return 0;
}

