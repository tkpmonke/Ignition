#include "skybox.hpp"
#include "modules/rendering/meshrenderer.hpp"
#include "shapes/cube.hpp"

const char* skybox_vert =
"#version 330\n"
"layout(location=0) in vec3 pos;"
"out vec3 texcoord;"
"uniform mat4 projection;"
"uniform mat4 model;"
"void main() {"
"  gl_Position = projection * model * vec4(pos,1);"
"  texcoord = pos;"
"}\0";

const char* skybox_frag =
"#version 330\n"
"in vec3 texcoord;"
"uniform samplerCube tex;"
"out vec4 o;"
"void main() {"
"  o = texture(tex, texcoord);"
"}\0";

namespace Ignition {
   Skybox::Skybox(std::vector<std::string> textures) {
      o = Object();
      tex.SetType(IGNITION_CUBE_MAP);
      tex.SetFlags(Rendering::Linear);
      tex.LoadData(textures);

      Rendering::Shader s = Rendering::Shader(skybox_vert, skybox_frag, Rendering::Unlit);
      s.albedo = tex;

      Rendering::MeshRenderer m;
      m.LoadShader(s);
      m.LoadModel(cube_model);
      
      std::shared_ptr<Rendering::MeshRenderer> ptr = std::make_shared<Rendering::MeshRenderer>(m);
      o.AddModule(ptr);
   }

   Skybox::Skybox(std::vector<const char*> textures, int w, int h, int nr, std::string name) {
      o = Object();
      tex.SetType(IGNITION_CUBE_MAP);
      tex.SetFlags(Rendering::Linear);
      tex.LoadData(textures, w, h, nr, name);

      Rendering::Shader s = Rendering::Shader(skybox_vert, skybox_frag, Rendering::Unlit);
      s.albedo = tex;

      Rendering::MeshRenderer m;
      m.LoadShader(s);
      m.LoadModel(cube_model);
      
     std::shared_ptr<Rendering::MeshRenderer> ptr = std::make_shared<Rendering::MeshRenderer>(m);
      o.AddModule(ptr);
   }

   void Skybox::Render() {
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
      o.transform.position = Ignition::MainCamera::camera->transform.position;
      
      Rendering::MeshRenderer* ptr = (Rendering::MeshRenderer*)o.GetModule("Mesh Renderer");

      glUseProgram(ptr->shader.program);

      glActiveTexture(GL_TEXTURE0 ); 
      glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
      ptr->shader.SetInt(0, "tex");
      ptr->transform = &o.transform;
      //ptr->Update();

      Matrix4 model = Matrix4(1.f);
      model = glm::translate(model, o.transform.position);
      ptr->shader.SetMatrix4(model, "model");

      ptr->shader.SetMatrix4(Ignition::MainCamera::camera->view_projection(), "projection");

      glBindVertexArray(ptr->vao);
      glDrawElements(GL_TRIANGLES, ptr->model.indices.size(), GL_UNSIGNED_INT, 0);

      //o.Update();
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
   }
}
