#include "modules/physics/collider.hpp"
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
namespace Ignition::Physics {
   void Collider::CreateCube(Vec3 size) {
      shape = new BoxShape(size);
      shapeVariables.size = size;
      shapeType = Cube;
   }

   void Collider::CreateSphere(float radius) {
      shape = new SphereShape(radius);
      shapeVariables.radius = radius;
      shapeType = Sphere;
   }
   
   void Collider::CreateMesh(std::vector<float>* vertices, std::vector<uint>* indices) {
      Array<Vec3> v;

      for (int i = 0; i < (int)indices->size(); i+=3) {
         v.push_back(Vec3(vertices->at(indices->at(i)), vertices->at(indices->at(i+1)), vertices->at(indices->at(i+2))));
      }

      ConvexHullShapeSettings meshSettings(v);

      // Use Create() to construct the shape
      Shape::ShapeResult result = meshSettings.Create();
      if (result.HasError()) {
          Ignition::IO::Error((std::string)"Error creating mesh collider: " + result.GetError().c_str());
          CreateEmpty();
          return;
      }

      // Assign the created shape
      shape = result.Get(); // shape is likely a Ref<Shape>

      // Set shape type
      shapeType = Mesh;
   }

   void Collider::CreateEmpty() {
      shape = new EmptyShape();
      shapeType = Empty;
   }
}
