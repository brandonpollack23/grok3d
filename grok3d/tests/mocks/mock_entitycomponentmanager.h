//
// Created by brpol on 8/4/18.
//

#ifndef GROK3DTESTS_MOCK_ENTITYCOMPONENTMANAGER_H
#define GROK3DTESTS_MOCK_ENTITYCOMPONENTMANAGER_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "grok3d/grok3d_types.h"
#include "grok3d/ecs/component/TransformComponent.h"

using namespace Grok3d;

class MockECM {
 public:
  MOCK_METHOD1(DeleteEntity, GRK_Result(GRK_Entity entity));
  MOCK_CONST_METHOD1(GetEntityComponentsBitMask, GRK_ComponentBitMask(GRK_Entity entity));

  /** Template methods need to be explicitly specialized to be mocked into tests */
  MOCK_METHOD2(AddComponentTransform, GRK_Result(GRK_Entity entity, GRK_TransformComponent component));
  template<class ComponentType>
  GRK_Result AddComponent(GRK_Entity entity, ComponentType component) { return GRK_Result::Ok; }

  MOCK_METHOD1(RemoveComponentTransform, GRK_Result(GRK_Entity entity));
  MOCK_METHOD1(RemoveComponentVoid, GRK_Result(GRK_Entity entity));
  template<class ComponentType>
  GRK_Result RemoveComponent(GRK_Entity entity) { return GRK_Result::Ok; }

  MOCK_METHOD1(
      GetComponentTransform,
      GRK_ComponentHandle<GRK_TransformComponent, MockECM>(GRK_Entity entity));
  template<class ComponentType>
  GRK_ComponentHandle<ComponentType, MockECM>
  GetComponent(GRK_Entity entity) {
    return GRK_ComponentHandle<ComponentType, MockECM>(this, nullptr, 0);
  }

  template<class ComponentType>
  static auto GetComponentTypeAccessIndex() -> size_t {
    return 2;
  }
};

/** Add Component Specialization for transform. */
template<> GRK_Result MockECM::AddComponent(GRK_Entity entity, GRK_TransformComponent component) {
  return AddComponentTransform(entity, std::forward<GRK_TransformComponent>(component));
}

template<> GRK_Result MockECM::RemoveComponent<GRK_TransformComponent>(GRK_Entity entity) {
  return RemoveComponentTransform(entity);
}
template<> GRK_Result MockECM::RemoveComponent<void>(GRK_Entity entity) {
  return RemoveComponentVoid(entity);
}

template<>
GRK_ComponentHandle<GRK_TransformComponent, MockECM>
MockECM::GetComponent<GRK_TransformComponent>(GRK_Entity entity) {
  return GetComponentTransform(entity);
}

#endif //GROK3DTESTS_MOCK_ENTITYCOMPONENTMANAGER_H
