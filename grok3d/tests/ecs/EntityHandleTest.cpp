/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "grok3d/ecs/entity/EntityHandle.h"
#include "grok3d/ecs/component/TransformComponent.h"

using namespace Grok3d;
using namespace ::testing;

int constexpr kTestEntityId = 37;

class MockECM {
 public:
  MOCK_METHOD1(DeleteEntity, GRK_Result(GRK_Entity entity));
  MOCK_METHOD1(GetEntityComponentsBitMask, GRK_ComponentBitMask(GRK_Entity entity));

  /** Template methods need to be explicitly specialized to be mocked into tests */
  MOCK_METHOD2(AddComponentTransform, GRK_Result(GRK_Entity entity, GRK_TransformComponent component));
  template<class ComponentType>
  GRK_Result AddComponent(GRK_Entity entity, ComponentType component) { return GRK_Result::Ok; }

  MOCK_METHOD1(RemoveComponentTransform, GRK_Result(GRK_Entity entity));
  template<class ComponentType>
  GRK_Result RemoveComponent(GRK_Entity entity) { return GRK_Result::Ok; }

  MOCK_METHOD1(
      GetComponentTransform,
      GRK_ComponentHandle<GRK_TransformComponent, MockECM>(GRK_Entity entity));
  template<class ComponentType>
  GRK_ComponentHandle<ComponentType, MockECM>
      GetComponent(GRK_Entity entity) {
    return GRK_ComponentHandle<ComponentType, MockECM>(this, nullptr, kTestEntityId);
  }
};

/** Add Component Specialization for transform. */
template<> GRK_Result MockECM::AddComponent(GRK_Entity entity, GRK_TransformComponent component) {
  return AddComponentTransform(entity, std::forward<GRK_TransformComponent>(component));
}

template<> GRK_Result MockECM::RemoveComponent<GRK_TransformComponent>(GRK_Entity entity) {
  return RemoveComponentTransform(entity);
}

template<>
GRK_ComponentHandle<GRK_TransformComponent, MockECM>
    MockECM::GetComponent<GRK_TransformComponent>(GRK_Entity entity) {
  return GetComponentTransform(entity);
}

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

