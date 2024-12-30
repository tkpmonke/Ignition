#include "modules/physics/collider.hpp"

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
   
   void Collider::CreateMesh(std::vector<float>& vertices, std::vector<uint>& indices) {
      auto verts = Convert3DataArrType<float, Float3>(vertices);
      auto tris = Convert3DataArrType<uint, IndexedTriangle>(indices);

      auto v = VertexList((const Float3*)verts.begin().base(), (const Float3*)verts.end().base());
      auto t = IndexedTriangleList((const IndexedTriangle*)tris.begin().base(), (const IndexedTriangle*)tris.end().base());

      auto meshSettings = MeshShapeSettings(v, t);
      Shape::ShapeResult out;
      shape = new MeshShape(meshSettings, out);
   }

   void Collider::CreateEmpty() {
      shape = new EmptyShape();
      shapeType = Empty;
   }
}
