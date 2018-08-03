/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

#include "gtest/gtest.h"
#include "grok3d/ecs/entity/EntityHandle.h"
#include "grok3d/ecs/EntityComponentManager.h"

using namespace Grok3d;

int constexpr kTestEntityId = 37;

class TestEntityHandle : public ::testing::Test {
 protected:
  GRK_EntityComponentManager ecm_;
  GRK_EntityHandle testEntity_;

  TestEntityHandle()
      : testEntity_(&ecm_, kTestEntityId) {}
};

TEST_F(TestEntityHandle, testEntityNotDestroyed) {
  EXPECT_EQ(testEntity_.IsDestroyed(), false);
}
