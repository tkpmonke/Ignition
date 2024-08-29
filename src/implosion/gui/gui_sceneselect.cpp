#include "gui/gui.hpp"
#include "scene.hpp"
#include "modules/rendering/meshrenderer.hpp"

#include <iostream>
#include <stdio.h>

void getRayFromMouse(float mouseX, float mouseY, int windowWidth, int windowHeight, 
                          const glm::mat4& projectionMatrix, const Ignition::Matrix4& viewMatrix,
                     Ignition::Vector3* rayWorld, Ignition::Vector3* rayDir) {
   glm::vec2 ndc;
    ndc.x = (2.0f * mouseX) / windowWidth - 1.0f;
    ndc.y = 1.0f - (2.0f * mouseY) / windowHeight;

    // Convert NDC to clip space (z=1 for the ray direction)
    glm::vec4 rayNdc(ndc.x, ndc.y, 1.0f, 1.0f);

    // Convert clip space to eye space (inverse projection matrix)
    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayNdc;
    rayEye.z = -1.0f; // The direction of the ray
    rayEye.w = 0.0f;  // We are working with direction, not position

    // Convert eye space to world space (inverse view matrix)
    *rayDir = glm::normalize(glm::vec3(glm::inverse(viewMatrix) * rayEye));
}

bool CheckRayCubeIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                      const glm::vec3& boxMin, const glm::vec3& boxMax) {
    float tmin = (boxMin.x - rayOrigin.x) / rayDirection.x;
    float tmax = (boxMax.x - rayOrigin.x) / rayDirection.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (boxMin.y - rayOrigin.y) / rayDirection.y;
    float tymax = (boxMax.y - rayOrigin.y) / rayDirection.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (boxMin.z - rayOrigin.z) / rayDirection.z;
    float tzmax = (boxMax.z - rayOrigin.z) / rayDirection.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    return true;
}

bool CheckRayMeshIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                              Ignition::Rendering::MeshRenderer* model) {
   Ignition::Vector3 cubeMax = Ignition::Vector3(std::numeric_limits<float>::min()),
                     cubeMin = Ignition::Vector3(std::numeric_limits<float>::max());

   for (const float& f : model->model.vertices) {
      cubeMin = glm::min(cubeMin, f); 
      cubeMax = glm::max(cubeMax, f); 
   }

   cubeMin += model->transform->position; 
   cubeMax += model->transform->position; 

   printf("MIN : X=%f Y=%f Z=%f\n", cubeMin.x, cubeMin.y, cubeMin.z); 
   printf("MAX : X=%f Y=%f Z=%f\n\n", cubeMax.x, cubeMax.y, cubeMax.z); 

   return CheckRayCubeIntersection(rayOrigin, rayDirection, cubeMin, cubeMax);
}


namespace Implosion {
   void GUI::RayCastMouse() {
      std::cout << "CLICKED\n";
      int w,h;
      double x,y;

      glfwGetWindowSize((GLFWwindow*)*this->window, &w,&h);
      glfwGetCursorPos((GLFWwindow*)*window, &x, &y);

      std::cout << x << "\n\n"; 

      glm::vec3 rayOrigin, rayDirection;
      getRayFromMouse(x, y, camera->size.x, camera->size.y, this->camera->ProjectionMatrix(), this->camera->ViewMatrix(), 
            &rayOrigin, &rayDirection);

      rayOrigin = this->camera->transform.position;
    
      printf("ORG : X=%f Y=%f Z=%f\n", rayOrigin.x, rayOrigin.y, rayOrigin.z); 
      printf("DIR : X=%f Y=%f Z=%f\n\n", rayDirection.x, rayDirection.y, rayDirection.z); 
      
      float distance;
      glm::vec3 intersectionPoint;

      for (auto& obj : *Ignition::scene.GetObjects())
      {
         Ignition::Rendering::MeshRenderer* mr = (Ignition::Rendering::MeshRenderer*)obj.GetModule("Mesh Renderer");
         if (mr == nullptr)
            continue;


         if (CheckRayMeshIntersection(this->camera->transform.position, rayDirection,
                  mr)) {
            std::cout << "Intersection detected at distance: " << distance << std::endl;
         }
      }
   }
}
