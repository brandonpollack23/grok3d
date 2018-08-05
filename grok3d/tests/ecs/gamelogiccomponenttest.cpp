/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "grok3d/grok3d.h"
#include "grok3d/grok3d_types.h"

using namespace Grok3d;
using namespace testing;

static constexpr auto kTestEntityID = static_cast<GRK_Entity>(37);
static auto kTestEntity =
    GRK_EntityHandle(std::make_unique<GRK_EntityComponentManager>().get(), kTestEntityID);

class TestBehaviour : public GRK_GameBehaviourBase {
 public:
  TestBehaviour(GRK_EntityHandle owningEntity) : GRK_GameBehaviourBase(owningEntity) {}

  MOCK_METHOD1(Update, void(double dt));
};

TEST(GameLogicComponentTests, TestRegisterBehaviour) {
  auto testGLC = GRK_GameLogicComponent();

  auto handle = testGLC.RegisterBehaviour(std::make_unique<TestBehaviour>(kTestEntity));
  EXPECT_EQ(handle, 1);

  handle = testGLC.RegisterBehaviour(std::make_unique<TestBehaviour>(kTestEntity));
  EXPECT_EQ(handle, 2);
}

TEST(GameLogicComponentTests, TestUpdateExistingBehaviour) {
  auto testGLC = GRK_GameLogicComponent();

  // Create 100 behaviours.
  for (auto i = 0; i < 100; i++) {
    auto testBC_ptr = std::make_unique<TestBehaviour>(kTestEntity);
    EXPECT_CALL(*testBC_ptr, Update(5));
    testGLC.RegisterBehaviour(std::move(testBC_ptr));
  }

  // Dispatch update call to 100 behaviours.
  testGLC.Update(5);
}

TEST(GameLogicComponentTests, TestBehaviourRemoval) {
  auto testGLC = GRK_GameLogicComponent();
  GRK_GameLogicComponent::BehaviourHandle behaviourHandles[100];

  // Create 100 behaviours.
  for (auto i = 0; i < 100; i++) {
    auto testBC_ptr = std::make_unique<TestBehaviour>(kTestEntity);

    if (i % 2 == 0) {
      EXPECT_CALL(*testBC_ptr, Update(5)).Times(1);
    } else {
      EXPECT_CALL(*testBC_ptr, Update(5)).Times(2);
    }

    behaviourHandles[i] = testGLC.RegisterBehaviour(std::move(testBC_ptr));
  }

  // Call update on all of them once, regular behaviour pattern.
  testGLC.Update(5);

  // Remove even numbered behaviours.
  for (auto i = 0; i < 50; i++) {
    testGLC.EnqueueBehaviourRemoval(behaviourHandles[2 * i]);
  }

  // Only odd ones should update as seen in creation step.
  testGLC.Update(5);
}
