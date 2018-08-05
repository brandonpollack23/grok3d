/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "grok3d/ecs/entity/EntityHandle.h"
#include "grok3d/ecs/component/TransformComponent.h"

#include "grok3d/tests/mocks/mock_entitycomponentmanager.h"

using namespace Grok3d;
using namespace ::testing;

auto constexpr kTestEntityId = 37;

class TestEntityHandle : public Test {
 protected:
  MockECM ecm_;
  GRK_EntityHandle__<MockECM> testEntity_;

  TestEntityHandle()
      : testEntity_(&ecm_, kTestEntityId) {}
};

/// New entities should not be destroyed.
TEST_F(TestEntityHandle, TestEntityNotDestroyed) {
  EXPECT_EQ(testEntity_.IsDestroyed(), false);
}

// TODO googlemock ecm_ and expect destroy to be called.
/// Destroyed entities should be destroyed.
TEST_F(TestEntityHandle, TestDestroyEntity) {
  EXPECT_CALL(ecm_, DeleteEntity(kTestEntityId));

  testEntity_.Destroy();
  EXPECT_EQ(testEntity_.IsDestroyed(), true);
}

TEST_F(TestEntityHandle, TestAddComponent) {
  EXPECT_CALL(ecm_, AddComponentTransform(kTestEntityId, A<GRK_TransformComponent>()));

  testEntity_.AddComponent(GRK_TransformComponent());
}

TEST_F(TestEntityHandle, TestAddComponent_entityDestroyed) {
  EXPECT_CALL(ecm_, AddComponentTransform(kTestEntityId, A<GRK_TransformComponent>()))
      .Times(0);

  testEntity_.Destroy();
  auto result = testEntity_.AddComponent(GRK_TransformComponent());
  EXPECT_EQ(result, GRK_Result::EntityAlreadyDeleted);
}

TEST_F(TestEntityHandle, TestRemoveComponent) {
  testEntity_.AddComponent(GRK_TransformComponent());

  EXPECT_CALL(ecm_, RemoveComponentTransform(kTestEntityId));
  testEntity_.RemoveComponent<GRK_TransformComponent>();
}

TEST_F(TestEntityHandle, TestGetComponent) {
  EXPECT_CALL(ecm_, GetComponentTransform(kTestEntityId))
      .WillOnce(Return(
          GRK_ComponentHandle<GRK_TransformComponent, MockECM>(&ecm_, nullptr, kTestEntityId)));

  testEntity_.GetComponent<GRK_TransformComponent>();
}

TEST_F(TestEntityHandle, TestHasComponents) {
  EXPECT_CALL(ecm_, GetEntityComponentsBitMask(kTestEntityId));

  testEntity_.HasComponents(0);
}

// TODO fix
TEST_F(TestEntityHandle, TestEqualityOperator) {
  auto entityLeft = GRK_EntityHandle__<MockECM>(nullptr, 1);
  auto entityRight = GRK_EntityHandle__<MockECM>(nullptr, 1);

  EXPECT_EQ(entityLeft == entityRight, true);
//  EXPECT_EQ(entityLeft == 1, true);
//  EXPECT_EQ(entityRight == 1, true);
}

TEST_F(TestEntityHandle, TestHash) {
  EXPECT_EQ(
      std::hash<GRK_EntityHandle__<MockECM>>{}(testEntity_),
      std::hash<std::size_t>{}(kTestEntityId));
}

