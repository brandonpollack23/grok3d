//
// Created by brpol on 7/26/18.
//

#ifndef GROK3DTESTS_MOVEBACKANDFORTHBEHAVIOUR_H
#define GROK3DTESTS_MOVEBACKANDFORTHBEHAVIOUR_H

#include "grok3d/grok3d.h"

#include <iostream>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

class MoveBackAndForthBehaviour : public GRK_GameBehaviourBase {
 public:
  explicit MoveBackAndForthBehaviour(GRK_EntityHandle entity) noexcept :
      GRK_GameBehaviourBase(entity),
      m_transform(entity.GetComponent<GRK_TransformComponent>()) {
  }

  auto Update(double dt) -> void override {
    static const float speed = 1.0f;

    float translationX = m_direction * speed * static_cast<float>(dt);

    m_updateCount++;

    m_transform->TranslateLocal(translationX, 0, 0);

    glm::dvec3 worldPosition = m_transform->GetWorldPosition();

    if (abs(worldPosition.x) > 1 * m_behaviourHandle) {
      //m_direction = -m_direction;
      UnregisterThisBehaviour();
      m_owningEntity.Destroy();
    }

    if (m_updateCount == 72) {
      std::cout << "Entity #" << m_owningEntity << ": " << worldPosition.x << ", " << worldPosition.y << ", "
                << worldPosition.z << std::endl;
      m_updateCount = 0;
    }
  }

 private:
  GRK_ComponentHandle<GRK_TransformComponent> m_transform;
  float m_direction = 1;
  int m_updateCount = 0;
};

#endif //GROK3DTESTS_MOVEBACKANDFORTHBEHAVIOUR_H
