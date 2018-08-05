/* Copyright (c) 2018 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "grok3d/ecs/component/ComponentHandle.h"

#include "grok3d/tests/mocks/mock_entitycomponentmanager.h"

using namespace testing;

auto constexpr kTestEntityId = 37;

class TestComponentHandle : public Test {
 protected:
  MockECM ecm_;

  /// Doesn't matter what the component type is so I used void.
  GRK_ComponentHandle<void, MockECM> testComponent_;

  TestComponentHandle()
      : testComponent_(&ecm_, reinterpret_cast<const void*>(0x12345), kTestEntityId) {}
};

TEST_F(TestComponentHandle, TestGetOwningEntity) {
  EXPECT_EQ(testComponent_.GetOwningEntity(), kTestEntityId);
}

TEST_F(TestComponentHandle, TestIsHandleValid_WhenValid) {
  EXPECT_CALL(ecm_, GetEntityComponentsBitMask(kTestEntityId)).WillOnce(Return(0b0111));

  EXPECT_EQ(testComponent_.IsHandleValid(), true);
}

TEST_F(TestComponentHandle, TestIsHandleValid_WhenNotValid) {
  EXPECT_CALL(ecm_, GetEntityComponentsBitMask(kTestEntityId)).WillOnce(Return(0b0011));

  EXPECT_EQ(testComponent_.IsHandleValid(), false);
}

TEST_F(TestComponentHandle, TestDestroy) {
  EXPECT_CALL(ecm_, RemoveComponentVoid(kTestEntityId));
  testComponent_.Destroy();
}
