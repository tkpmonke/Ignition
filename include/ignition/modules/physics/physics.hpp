#pragma once

#define IGNITION_PHYSICS_HPP

#include <memory>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/EmptyShape.h>
#include <Jolt/Physics/Collision/ShapeCast.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <GLFW/glfw3.h>

#include "layers.hpp"

using namespace JPH;

namespace Ignition::Physics {
   class World {
   public:
      float deltaTime;
      float fixedTimeStep = (float)1/120;
      int sampleRate;

      int maxBodies = 10000;
      int maxBodyPairs = 65536;
      int numBodyMutexes = 8;
      int maxContactConstraints = 65536;

      friend class Rigidbody;
      friend class Colider;

      void Init();

      void Update(std::function<void()> physicsUpdate, std::function<void()> latePhysicsUpdate);

      void Shutdown();

      void OptimizeCollisionPerformace();

   private:
      PhysicsSystem physicsSystem;
      std::unique_ptr<TempAllocatorImpl> allocator;
      std::unique_ptr<JobSystemThreadPool> jobSystem;

      BPLayerInterfaceImpl broad_phase_layer_interface;
	   ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
	   ObjectLayerPairFilterImpl object_vs_object_layer_filter;

   };

   extern std::shared_ptr<World> physicsWorld;
}
