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

      void Init() {
         RegisterDefaultAllocator();
         Factory::sInstance = new Factory();
         RegisterTypes();

         allocator = std::unique_ptr<TempAllocatorImpl>(new TempAllocatorImpl(100*1024*1024));

         jobSystem = std::unique_ptr<JobSystemThreadPool>(new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1));

         physicsSystem.Init(
            maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints,
            broad_phase_layer_interface,
            object_vs_broadphase_layer_filter,
            object_vs_object_layer_filter
         );
         
      }

      void Update(std::function<void()> physicsUpdate, std::function<void()> latePhysicsUpdate) {
         static float pt = 0;
         float time = glfwGetTime();
         this->deltaTime = time-pt;
         pt = time;

         static float accumulator = 0;
         accumulator += deltaTime;

         while (accumulator >= fixedTimeStep) {
            physicsUpdate();
            accumulator -= fixedTimeStep;

            physicsSystem.Update(fixedTimeStep, 1, allocator.get(), jobSystem.get());
            latePhysicsUpdate();
         }
      }

      void Shutdown() {
         UnregisterTypes();
         delete Factory::sInstance;
         Factory::sInstance = nullptr;
      }

      void OptimizeCollisionPerformace() {
         physicsSystem.OptimizeBroadPhase();
      }

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
