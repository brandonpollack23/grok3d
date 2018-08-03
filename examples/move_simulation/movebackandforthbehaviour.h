//
// Created by brpol on 7/26/18.
//

#ifndef GROK3DTESTS_MOVEBACKANDFORTHBEHAVIOUR_H
#define GROK3DTESTS_MOVEBACKANDFORTHBEHAVIOUR_H

#include "grok3d/grok3d.h"

#include <iostream>

using namespace Grok3d;

class MoveBackAndForthBehaviour : public GRK_GameBehaviourBase {
 public:
  explicit MoveBackAndForthBehaviour(GRK_EntityHandle entity) noexcept :
      GRK_GameBehaviourBase(entity),
      m_transform(entity.GetComponent<GRK_TransformComponent>()) {
  }

  auto Update(double dt) -> void override {
    static const float speed = 1.0f;

    float translationX = direction_ * speed * static_cast<float>(dt);

    updateCount_++;

    m_transform->TranslateLocal(translationX, 0, 0);

    glm::dvec3 worldPosition = m_transform->GetWorldPosition();

    if (abs(worldPosition.x) > 1 * direction_) {
      //direction_ = -direction_;
      UnregisterThisBehaviour();
      owningEntity_.Destroy();
    }

    if (updateCount_ == 72) {
      std::cout << "Entity #" << owningEntity_ << ": " << worldPosition.x << ", " << worldPosition.y << ", "
                << worldPosition.z << std::endl;
      updateCount_ = 0;
    }
  }

 private:
  GRK_ComponentHandle<GRK_TransformComponent> m_transform;
  float direction_ = 1;
  int updateCount_ = 0;
};

#endif //GROK3DTESTS_MOVEBACKANDFORTHBEHAVIOUR_H
