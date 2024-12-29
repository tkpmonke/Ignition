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
#include <Jolt/Physics/Collision/Shape/EmptyShape.h>
#include <Jolt/Physics/Collision/ShapeCast.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <memory>
#include <vector>
#include <functional>

#ifdef DEBUG
#include <iostream>
#endif

using namespace JPH;

#define vels_impulse 0
#define vels_force 1

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
         return true;
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
         return true;
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

   static void DebugCallback(const char* message, ...) {
      std::cout << ("JOLT > "+(std::string)message);
   };

   static void RegisterDebugCallback() {
      JPH::Trace = &DebugCallback;
      
   }

   class World {
   public:
      float deltaTime;
      float fixedTimeStep = (float)1/60;
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

         allocator = std::unique_ptr<TempAllocatorImpl>(new TempAllocatorImpl(120*1024*1024));

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

      // optimizes broadphase
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

   template<typename From, typename To>
   std::vector<To> Convert3DataArrType(const std::vector<From>& In) {
       if (In.size() % 3 != 0) {
           throw std::runtime_error("Input vector size is not a multiple of 3!");
       }
   
       std::vector<To> Out;
       Out.reserve(In.size() / 3);
   
       for (size_t i = 0; i < In.size(); i += 3) {
          Out.emplace_back(In[i], In[i + 1], In[i + 2]);
       }
   
       return Out;
   }

   class Collider {
   public:

      enum ShapeType {
         Cube = 1,
         Sphere = 2,
         Mesh = 3,
         Empty = 0
      } shapeType = Empty;

      union ShapeVariables {
         float radius;
         Vec3 size;
      } shapeVariables;

      void CreateCube(Vec3 size) {
         shape = new BoxShape(size);
         shapeVariables.size = size;
         shapeType = Cube;
      }

      void CreateSphere(float radius) {
         shape = new SphereShape(radius);
         shapeVariables.radius = radius;
         shapeType = Sphere;
      }

      void CreateMesh(std::vector<float>& vertices, std::vector<uint>& indices) {
         auto verts = Convert3DataArrType<float, Float3>(vertices);
         auto tris = Convert3DataArrType<uint, IndexedTriangle>(indices);

         auto v = VertexList((const Float3*)verts.begin().base(), (const Float3*)verts.end().base());
         auto t = IndexedTriangleList((const IndexedTriangle*)tris.begin().base(), (const IndexedTriangle*)tris.end().base());

         auto meshSettings = MeshShapeSettings(v, t);
         Shape::ShapeResult out;
         shape = new MeshShape(meshSettings, out);
      }

      void CreateEmpty() {
         shape = new EmptyShape();
         shapeType = Empty;
      }

      friend class Rigidbody;
   private:
      Shape* shape = nullptr;

   };

   class Rigidbody {
   public:
      float mass = 1;
      
      Vec3 velocity = Vec3(0,0,0);
      Vec3 position = Vec3(0,0,0);
      Quat rotation = Quat::sIdentity();

      float bounciness = 0.1f;

      bool _static = false;
      bool trigger = false;

      Collider collider;

      Rigidbody() = default;

      void AddCollider(Collider c) {
         BodyInterface& bodyInterface = world->physicsSystem.GetBodyInterface();
         
         if (body != nullptr) {
            bodyInterface.RemoveBody(body->GetID());
            bodyInterface.DestroyBody(body->GetID());
            body = nullptr;
         }
         this->collider = c;
         EMotionType motionType = _static ? EMotionType::Static : EMotionType::Dynamic;
         int layer = _static ? Layers::NonMoving : Layers::Moving;

         
         BodyCreationSettings settings(
             collider.shape,
             position,
             rotation,
             motionType,
             layer
         );
         
         body = bodyInterface.CreateBody(settings);
         if (body == nullptr) {
            std::cerr << "Failed to create body!\n";
            return;
         }
         bodyInterface.AddBody(body->GetID(), EActivation::Activate);

      }

      void RefreshVariables() {
         BodyInterface& bodyInterface = world->physicsSystem.GetBodyInterface();
         
         if (body) {
             RVec3 position = body->GetPosition();
             Quat rotation = body->GetRotation();
             auto* shape = body->GetShape();
         
             bodyInterface.DestroyBody(body->GetID());

             EMotionType motionType = _static ? EMotionType::Static : EMotionType::Dynamic;
             int layer = _static ? Layers::NonMoving : Layers::Moving;
         
             BodyCreationSettings settings(
                 shape,
                 position,
                 rotation,
                 motionType,
                 layer
             );
         
             body = bodyInterface.CreateBody(settings);
             bodyInterface.AddBody(body->GetID(), EActivation::Activate);
         }
      }

      void Init(std::shared_ptr<World> world) {
         this->world = world;

         Collider c;
         c.CreateEmpty();
         AddCollider(c);

         std::cout << "init\n";
      }

      virtual void UpdateFixed() {
         BodyInterface& bodyInterface = world->physicsSystem.GetBodyInterface();

         if (body != nullptr ) {
            if (!_static) {
               body->SetLinearVelocity(this->velocity);
               body->SetIsSensor(this->trigger);
               bodyInterface.SetGravityFactor(body->GetID(), this->mass);

               bodyInterface.SetRestitution(body->GetID(), bounciness);
               
            }
            bodyInterface.SetPositionAndRotationWhenChanged(
               body->GetID(), this->position, this->rotation, EActivation::Activate);
         }
      }

      virtual void PostUpdateFixed() {
         BodyInterface& bodyInterface = world->physicsSystem.GetBodyInterface();
         if (body != nullptr/* && !_static*/) {
            this->velocity = bodyInterface.GetLinearVelocity(body->GetID());
            this->position = bodyInterface.GetCenterOfMassPosition(body->GetID());
            this->rotation = bodyInterface.GetRotation(body->GetID());
         }
      }

      void Delete() {
         BodyInterface& bodyInterface = world->physicsSystem.GetBodyInterface();
         if (body != nullptr) {
            bodyInterface.RemoveBody(body->GetID());
            bodyInterface.DestroyBody(body->GetID());
         }
      }



      void AddForce(Vec3 force, int type) {
         BodyInterface& bodyInterface = world->physicsSystem.GetBodyInterface();
         bodyInterface.ActivateBody(body->GetID());

         Vec3 vel = bodyInterface.GetLinearVelocity(body->GetID());

         if (type == 0) {
            vel += force;
         } else {
            vel += force*world->deltaTime;
         }

         this->velocity = vel;
      }
      

   private:
      Body* body = nullptr;
      std::shared_ptr<World> world;
   
   };
}
