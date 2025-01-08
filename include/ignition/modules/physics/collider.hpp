#pragma once

#include "physics.hpp"
#include "utils/io.hpp"

namespace Ignition::Physics {
   
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

      void CreateCube(Vec3 size);

      void CreateSphere(float radius);

      void CreateMesh(std::vector<float>* vertices, std::vector<uint>* indices);

      void CreateEmpty();

      Collider() {
         if (!Ignition::IO::InEditor()) {
            CreateEmpty();
         }
      }
   
      Ref<Shape> shape = nullptr;
   };
}
