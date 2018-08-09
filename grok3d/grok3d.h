/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file convenience header that includes the entire engine*/

#ifndef __GROK3D__H
#define __GROK3D__H

#include "grok3d_types.h"

#include "ecs/EntityComponentManager.h"

#include "ecs/entity/EntityHandle.h"

#include "ecs/component/ComponentHandle.h"
#include "ecs/component/TransformComponent.h"
#include "ecs/component/GameLogicComponent.h"
#include "ecs/component/RenderComponent.h"

#include "ecs/system/System.h"
#include "ecs/system/SystemManager.h"
#include "ecs/system/GameLogicSystem.h"
#include "ecs/system/RenderSystem.h"

#include "grok3d/shaders/shaderprogram.h"
#include "grok3d/textures/texturehandle.h"

#include "engine/Engine.h"

#endif

