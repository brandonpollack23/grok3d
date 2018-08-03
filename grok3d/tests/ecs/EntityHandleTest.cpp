/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "grok3d/ecs/entity/EntityHandle.h"
#include "grok3d/ecs/EntityComponentManager.h"

using namespace Grok3d;
using namespace ::testing;

int constexpr kTestEntityId = 37;

class MockECM {
 public:
  MOCK_METHOD1(DeleteEntity, GRK_Result(GRK_Entity entity));

  /** Template methods need to be explicitly specialized to be mocked into tests */
  MOCK_METHOD2(AddComponentTransform, GRK_Result(GRK_Entity entity, GRK_TransformComponent component));
  template<class ComponentType>
  GRK_Result AddComponent(GRK_Entity entity, ComponentType component) { return GRK_Result::Ok; }
};

/** Add Component Specialization for transform. */
template<> GRK_Result MockECM::AddComponent(GRK_Entity entity, GRK_TransformComponent component) {
  // TODO assert that component is RVALUE reference
  return AddComponentTransform(entity, std::forward<GRK_TransformComponent>(component));
}

class TestEntityHandle : public Test {
 protected:
  MockECM ecm_;
  GRK_EntityHandle__<MockECM> testEntity_;

  TestEntityHandle()
      : testEntity_(&ecm_, kTestEntityId) {}
};

/// New entities should not be destroyed.
TEST_F(TestEntityHandle, testEntityNotDestroyed) {
  EXPECT_EQ(testEntity_.IsDestroyed(), false);
}

// TODO googlemock ecm_ and expect destroy to be called.
/// Destroyed entities should be destroyed.
TEST_F(TestEntityHandle, testDestroyEntity) {
  EXPECT_CALL(ecm_, DeleteEntity(kTestEntityId));

  testEntity_.Destroy();
  EXPECT_EQ(testEntity_.IsDestroyed(), true);
}

TEST_F(TestEntityHandle, testAddComponent) {
  EXPECT_CALL(ecm_, AddComponentTransform(kTestEntityId, A<GRK_TransformComponent>()));

  testEntity_.AddComponent(GRK_TransformComponent());
}

TEST_F(TestEntityHandle, testAddComponent_entityDestroyed) {
  EXPECT_CALL(ecm_, AddComponentTransform(kTestEntityId, A<GRK_TransformComponent>()))
      .Times(0);

  testEntity_.Destroy();
  auto result = testEntity_.AddComponent(GRK_TransformComponent());
  EXPECT_EQ(result, GRK_Result::EntityAlreadyDeleted);
}
