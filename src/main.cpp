/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d.h"

#include <cmath>
#include <iostream>
#include <memory>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

auto PrintUsageAndExit() -> int
{
    std::cout << "Specify a test number to run:\n\
        1: MoveBackAndForth and delete" << std::endl;
    return 0;
}

class MoveBackAndForthBehavour : public GRK_GameBehaviourBase
{
    public:
        MoveBackAndForthBehavour(GRK_EntityHandle entity) noexcept : 
            GRK_GameBehaviourBase(entity),
            m_transform(entity.GetComponent<GRK_TransformComponent>())
        {
        }

        auto Update(double dt) -> void override
        {
            static const float speed = 1.0f;

            float translationX = m_direction * speed * static_cast<float>(dt);

            m_updateCount++;

            m_transform->TranslateLocal(translationX, 0, 0);

            glm::dvec3 worldPosition = m_transform->GetWorldPosition();

            if (abs(worldPosition.x) > 1 * m_behaviourHandle)
            {
                //m_direction = -m_direction;
                UnregisterThisBehaviour();
                m_owningEntity.Destroy();
            }

            if (m_updateCount == 72)
            { 
                std::cout << "Entity #" << m_owningEntity << ": " << worldPosition.x << ", " << worldPosition.y << ", " << worldPosition.z << std::endl;
                m_updateCount = 0;
            }
        }

    private:
        GRK_ComponentHandle<GRK_TransformComponent> m_transform;
        float m_direction = 1;
        int m_updateCount = 0;
};

auto CreateAndRegisterMoveTestEntity(GRK_EntityComponentManager& ecm) -> GRK_Result
{
    auto pointEntity = ecm.CreateEntity();

    auto moveBehaviour = std::make_unique<MoveBackAndForthBehavour>(pointEntity);

    GRK_GameLogicComponent glc;
    glc.RegisterBehaviour(std::move(moveBehaviour));

    return pointEntity.AddComponent(std::move(glc));
}

auto CreationMovementDeleteTest() -> void
{
    auto engineInitialization = [](GRK_EntityComponentManager& ecm) -> GRK_Result
    {
        for (int i = 0; i < 5; i++)
        {
            auto result = CreateAndRegisterMoveTestEntity(ecm);
            if (result != GRK_Result::Ok)
            {
                return result;
            }
        }

        return GRK_Result::Ok;
    };

    GRK_Engine engine(engineInitialization);
    engine.Run();
}

auto main(int argc, char* argv[]) -> int
{
    std::cout << "Running Grok3d tests" << std::endl;

    if(argc < 2)
    {
        return PrintUsageAndExit();
    }
    
    switch(*argv[1])
    {
    case '1':
        CreationMovementDeleteTest();
    default:
        std::cout << "no such test: " << argv[1] << std::endl;
    }

    return 0;
}
