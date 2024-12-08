#pragma once

#include "GLFW/glfw3.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <memory>
#include <unordered_map>

JPH_SUPPRESS_WARNINGS_STD_BEGIN
void* JPH_Allocate(size_t inSize) { return malloc(inSize); }
void JPH_Free(void* inBlock) { free(inBlock); }
JPH_SUPPRESS_WARNINGS_STD_END

using namespace JPH;

namespace vels {
   namespace Layers {
      static constexpr ObjectLayer NonMoving = 0;
      static constexpr ObjectLayer Moving = 1;
      static constexpr ObjectLayer NumOfLayers = 2;
   }
   namespace BroadPhaseLayers
   {
	   static constexpr BroadPhaseLayer NON_MOVING(0);
	   static constexpr BroadPhaseLayer MOVING(1);
	   static constexpr uint NUM_LAYERS(2);
   };

   class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
   {
   public:
   	virtual bool ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override
   	{
   		switch (inObject1)
   		{
            case Layers::NonMoving:
   			return inObject2 == Layers::Moving; // Non moving only collides with moving
   		case Layers::Moving:
   			return true; // Moving collides with everything
   		default:
   			return false;
   		}
   	}
   };
   
   class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
   {
   public:
   	BPLayerInterfaceImpl()
   	{
   		// Create a mapping table from object to broad phase layer
   		mObjectToBroadPhase[Layers::NonMoving] = BroadPhaseLayers::NON_MOVING;
   		mObjectToBroadPhase[Layers::Moving] = BroadPhaseLayers::MOVING;
   	}
   
   	virtual uint GetNumBroadPhaseLayers() const override
   	{
   		return BroadPhaseLayers::NUM_LAYERS;
   	}
   
   	virtual BroadPhaseLayer	GetBroadPhaseLayer(ObjectLayer inLayer) const override
   	{
   		JPH_ASSERT(inLayer < Layers::NumOfLayers);
   		return mObjectToBroadPhase[inLayer];
   	}
   
   #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
   	virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
   	{
   		switch ((BroadPhaseLayer::Type)inLayer)
   		{
   		case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
   		case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
   		default:													JPH_ASSERT(false); return "INVALID";
   		}
   	}
   #endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
   
   private:
   	BroadPhaseLayer mObjectToBroadPhase[Layers::NumOfLayers];
   };

   class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
   {
   public:
   	virtual bool ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override
   	{
   		switch (inLayer1)
   		{
   		case Layers::NonMoving:
   			return inLayer2 == BroadPhaseLayers::MOVING;
   		case Layers::Moving:
   			return true;
   		default:
   			JPH_ASSERT(false);
   			return false;
   		}
   	}
   };

   class World {
   public:
      float deltaTime;
      int sampleRate;

      int maxBodies = 1024;
      int maxBodyPairs = 1024;
      int numBodyMutexes = 8;
      int maxContactConstraints = 1024;

      friend class Rigidbody;
      friend class Colider;

      void Init() {
         Factory::sInstance = new Factory();
         RegisterTypes();

         TempAllocatorImpl tempAllocator(10*1024*1024);
         JobSystemThreadPool jobSystem(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

         BPLayerInterfaceImpl broad_phase_layer_interface;
	      ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
	      ObjectLayerPairFilterImpl object_vs_object_layer_filter;

         physicsSystem.Init(
            maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints,
            broad_phase_layer_interface,
            object_vs_broadphase_layer_filter,
            object_vs_object_layer_filter
         );
         
         BodyInterface& bI = physicsSystem.GetBodyInterface();
      }

      void Update() {
         static float pt = 0;
         float time = glfwGetTime();
         this->deltaTime = time-pt;
         pt = time;

      }

      void Shutdown() {

      }

   private:
      PhysicsSystem physicsSystem;
   };

   class Collider {};

   class BoxCollider : public Collider { 
   
   };

   class Rigidbody {
   public:
      float mass = 1;
      std::shared_ptr<Collider> collider = nullptr;

      Rigidbody(World* wrld) {
      }
      

   private:
   };
}
