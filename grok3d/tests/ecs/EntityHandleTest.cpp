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
};

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
