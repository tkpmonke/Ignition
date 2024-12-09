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
#include <Jolt/Physics/Collision/ShapeCast.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <memory>
#include <unordered_map>
#include <vector>

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

         jobSystem = std::unique_ptr<JobSystemThreadPool>(new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1));

         BPLayerInterfaceImpl broad_phase_layer_interface;
	      ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
	      ObjectLayerPairFilterImpl object_vs_object_layer_filter;

         physicsSystem.Init(
            maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints,
            broad_phase_layer_interface,
            object_vs_broadphase_layer_filter,
            object_vs_object_layer_filter
         );
         
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
      TempAllocatorImpl allocator = TempAllocatorImpl(10*1024*1024);
      std::unique_ptr<JobSystemThreadPool> jobSystem;
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
         Cube,
         Sphere,
         Mesh
      } shapeType = Cube;

      void CreateCube(Vec3 size) {
         shape = new BoxShape(size);
         shapeType = Cube;
      }

      void CreateSphere(float radius) {
         shape = new SphereShape(radius);
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

      friend class Rigidbody;
   private:
      Shape* shape;

   };

   class Rigidbody {
   public:
      float mass = 1;
      Vec3 position = Vec3(0,0,0);
      Quat rotation = Quat::sIdentity();
      Vec3 velocity = Vec3(0,0,0);

      bool _static = false;

      std::shared_ptr<Collider> collider = nullptr;

      Rigidbody() = default;

      void AddCollider(Collider c) {
         collider = std::make_shared<Collider>(c);
         EMotionType motionType = _static ? EMotionType::Static : EMotionType::Dynamic;
         int layer = _static ? Layers::NonMoving : Layers::Moving;

         BodyInterface& bodyInterface = world->physicsSystem.GetBodyInterface();
         
         BodyCreationSettings settings(
             collider->shape,
             position,
             rotation,
             motionType,
             layer
         );
         
         Body* newBodyID = bodyInterface.CreateBody(settings);
         bodyInterface.AddBody(newBodyID->GetID(), EActivation::Activate);
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
         
             Body* newBodyID = bodyInterface.CreateBody(settings);
             bodyInterface.AddBody(newBodyID->GetID(), EActivation::Activate);
         }
      }

      void Init(World* world) {
         this->world = world;
      }
      

   private:
      Body* body;
      World* world;
   };
}
