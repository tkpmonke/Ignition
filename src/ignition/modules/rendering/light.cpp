#include "modules/rendering/light.hpp"
#include "utils/files.hpp"
#include "scene.hpp"
#include "camera.hpp"

using namespace Ignition::IO;
namespace Ignition::Rendering {
   std::vector<std::shared_ptr<Light>> lights;
   DirectionalLight directionalLight;
   int program = -2;

   void Light::Serialize() {
      Write8(type);
      WriteFloat(fallOff);
      WriteFloat(power);
      WriteVector3(ambient);
      WriteVector3(diffuse);
      WriteVector3(specular);

      if (type == Spot) {
         WriteFloat(cutOff);
         WriteFloat(outerCutOff);
      }
   }

   void Light::Deserialize() {
      type = (LightType)Read8();

      fallOff = ReadFloat();
      power = ReadFloat();

      ambient = ReadVector3();
      diffuse = ReadVector3();
      specular = ReadVector3();

      if (type == Spot) {
         cutOff = ReadFloat();
         outerCutOff = ReadFloat();
      }
   }

   std::string vert = 
      "#version 430 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform mat4 model;\n"
      "uniform mat4 projection;\n"
      "void main() {\n"
      "  gl_Position = projection * model * vec4(aPos, 1);\n"
      "}\0";

   std::string frag =
      "#version 430 core\n"
      "void main() {}\0";


   void RenderShadowMaps() {
      Camera cam = *Ignition::mainCamera;

      if (program == -2) {
         Shader s = Shader(vert, frag, ShaderType_None);
         program = s.program;
      }
      
      directionalLight.shadowMap.Bind();
      glClear(GL_DEPTH_BUFFER_BIT);

      Matrix4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 20.f);
      Matrix4 lightView = glm::lookAt(directionalLight.direction, Vector3(0), Vector3(0, 1, 0));
      Ignition::mainCamera->i_viewProj = lightProjection * lightView;
      Ignition::mainCamera->i_lightProj = lightProjection * lightView;

      Ignition::scene.Render(program);

      /*for (auto& l : lights) {
         l->shadowMap.Bind();
         glClear(GL_DEPTH_BUFFER_BIT);

         Ignition::mainCamera->transform = l->transform;

         Ignition::scene.Render();
      }*/
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      Ignition::mainCamera->i_viewProj = Matrix4(0);

      glViewport(0, 0, Ignition::mainCamera->size.x, Ignition::mainCamera->size.y);
   }
}
