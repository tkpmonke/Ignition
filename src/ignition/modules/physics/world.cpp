#include "modules/physics/physics.hpp"

namespace Ignition::Physics {
   void World::Init() {
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

      void World::Update(std::function<void()> physicsUpdate, std::function<void()> latePhysicsUpdate) {
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

      void World::Shutdown() {
         UnregisterTypes();
         delete Factory::sInstance;
         Factory::sInstance = nullptr;
      }

      void World::OptimizeCollisionPerformace() {
         physicsSystem.OptimizeBroadPhase();
      }
}
