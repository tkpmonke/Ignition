#include "gui/gui.hpp"
#include "scene.hpp"
#include "modules/rendering/meshrenderer.hpp"

void getRayFromMouse(float mouseX, float mouseY, int windowWidth, int windowHeight, 
                     const glm::mat4& projectionMatrix, const Ignition::Matrix4& viewMatrix,
                     Ignition::Vector3* rayDir) {
    
    // Convert mouse position from screen space to NDC
    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight;
    glm::vec4 rayNDC(x, y, -1.0f, 1.0f);

    // Convert NDC to eye space (inverse projection)
    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayNDC;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Z set to -1 for a forward direction ray, W set to 0 for direction

    // Convert to world space (inverse view matrix)
    glm::vec4 rayWorld4D = glm::inverse((glm::mat4)viewMatrix) * rayEye;

    // Extract the direction by normalizing the 3D component of the world ray
    *rayDir = -glm::normalize(glm::vec3(rayWorld4D));
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

   for (int i = 0; i < model->model.vertices.size(); i+=3) {
      Ignition::Vector3 f = { model->model.vertices[i],
                              model->model.vertices[i+1],
                              model->model.vertices[i+2]};

      cubeMin = glm::min(cubeMin, f); 
      cubeMax = glm::max(cubeMax, f); 
   }

   cubeMin += model->transform->position; 
   cubeMax += model->transform->position; 
   cubeMin *= model->transform->scale; 
   cubeMax *= model->transform->scale; 

   //printf("MIN : X=%f Y=%f Z=%f\n", cubeMin.x, cubeMin.y, cubeMin.z); 
   //printf("MAX : X=%f Y=%f Z=%f\n\n", cubeMax.x, cubeMax.y, cubeMax.z); 

   return CheckRayCubeIntersection(rayOrigin, rayDirection, cubeMin, cubeMax);
}


namespace Implosion {
   void GUI::RayCastMouse() {
      double x,y;

      glfwGetCursorPos((GLFWwindow*)*window, &x, &y);
      ImVec2 pos = ImGui::GetWindowPos();
      x -= pos.x;
      y -= pos.y;

      glm::vec3 rayDirection;
      getRayFromMouse(x, y, camera->size.x, camera->size.y, this->camera->ProjectionMatrix(), this->camera->ViewMatrix(), 
            &rayDirection);

      //printf("ORG : X=%f Y=%f Z=%f\n", rayOrigin.x, rayOrigin.y, rayOrigin.z); 
      //printf("DIR : X=%f Y=%f Z=%f\n\n", rayDirection.x, rayDirection.y, rayDirection.z); 
      
      float distance;
      glm::vec3 intersectionPoint;
      bool b = false;

      for (auto& obj : *Ignition::scene.GetObjects())
      {
         Ignition::Rendering::MeshRenderer* mr = (Ignition::Rendering::MeshRenderer*)obj.GetModule("Mesh Renderer");
         if (mr == nullptr)
            continue;


         if (CheckRayMeshIntersection(this->camera->transform.position, rayDirection,
                  mr)) {
            b = true;
            this->selectedObject = &obj;
         }
      }

      if (b == false)
         this->selectedObject = nullptr;
   }
}
