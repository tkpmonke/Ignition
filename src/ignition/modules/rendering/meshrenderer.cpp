#include "modules/rendering/meshrenderer.hpp"
#include "utils/files.hpp"
#include "shapes/cube.hpp"
#include "shapes/square.hpp"
#include "utils/model_loader.hpp"
#include "utils/default_shaders.hpp"
#include "textures/grid.hpp"

#define PI (float)3.14159265359

namespace Ignition::Rendering {
   int currentProgram = 999;
   int currentVao = 999;
   std::unordered_map<std::string, int> model_lookup_table;

   void MeshRenderer::LoadModel(Model m)
   {

      this->model = m;
      
      for (auto i : model_lookup_table)
      {
         if (i.first == model.name)
         {
            this->vao = i.second;
            return;
         }
      }

      unsigned int VBO, EBO;
      glGenVertexArrays(1, &this->vao);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);
      glBindVertexArray(this->vao);
      
      std::vector<float> buffer;

      for (int i = 0; i < this->model.vertices.size(); i+=3)
      {
         buffer.push_back(this->model.vertices[i]);
         buffer.push_back(this->model.vertices[i+1]);
         buffer.push_back(this->model.vertices[i+2]);
         buffer.push_back(this->model.normals[i]);
         buffer.push_back(this->model.normals[i+1]);
         buffer.push_back(this->model.normals[i+2]);
         buffer.push_back(this->model.uv[i]);
         buffer.push_back(this->model.uv[i+1]);
      }
            

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);
      

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->model.indices.size() * sizeof(int), &this->model.indices[0], GL_STATIC_DRAW);
      

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);


       model_lookup_table[m.name] = this->vao;
   }

   void MeshRenderer::Start() {
      camera = Ignition::MainCamera::camera;
   }
   
   void MeshRenderer::Update() {
      if (this->shader.program != currentProgram) {
         glUseProgram(this->shader.program);
         currentProgram = this->shader.program;
        
         if (camera->viewProj == Matrix4(0)) {
            camera->viewProj = camera->view_projection();
         }

         this->shader.SetMatrix4(camera->viewProj, "projection");
      }
      if (this->vao != currentVao) {
         glBindVertexArray(this->vao); 
         currentVao = this->vao;
      }


      Matrix4 model = Matrix4(1.f);

      model = glm::translate(model, this->transform->position);
      model *= glm::mat4_cast(glm::quat(glm::radians(this->transform->rotation)));
      model = glm::scale(model, this->transform->scale);

      this->shader.SetMatrix4(model, "model");
      glActiveTexture(GL_TEXTURE0 ); 
      if (shader.albedo.type == 2) {
         glBindTexture(GL_TEXTURE_2D, this->shader.albedo);
         this->shader.SetInt(0, "material.albedo");
      }
      this->shader.SetVec4(this->shader.color*this->shader.intensity, "material.color");
      
      if (this->shader.type == ShaderType::Lit)
      {
         this->shader.SetInt(this->shader.diffuse, "material.diffuse");
         this->shader.SetFloat(this->shader.shininess, "material.shininess");
         this->shader.SetVec3(this->shader.specular, "material.specular");
         glActiveTexture(GL_TEXTURE1); 
         glBindTexture(GL_TEXTURE_2D, this->shader.diffuse);
      }

      glDrawElements(GL_TRIANGLES, this->model.indices.size(), GL_UNSIGNED_INT, 0);
   }

   void MeshRenderer::Serialize() {
      Ignition::IO::WriteString(model.path);

      // for testing, the only shader that works is the default unlit
      Ignition::IO::WriteString("");

      Ignition::IO::Write8((int)shader.type);

      Ignition::IO::WriteFloat(shader.color.r);
      Ignition::IO::WriteFloat(shader.color.g);
      Ignition::IO::WriteFloat(shader.color.b);
      Ignition::IO::WriteFloat(shader.color.a);

      Ignition::IO::WriteFloat(shader.intensity);

      Ignition::IO::WriteString(shader.albedo.name);
   }

   void MeshRenderer::Deserialize() {
      std::string modelName = Ignition::IO::ReadString();
      if (modelName == "cube")
         LoadModel(cube_model);
      else if (modelName == "square")
         LoadModel(square_model);
      //else 
        // LoadModel(Ignition::ModelLoader::LoadModel(Ignition::IO::GetProjectHome() + "/" + modelName)[0]);

      std::string shaderName = Ignition::IO::ReadString();
      bool isLit = Ignition::IO::Read8(); 
      if (shaderName.empty())
      {
         Shader s = Shader(unlit_vertex, unlit_fragment, (ShaderType)isLit);
         LoadShader(s);
      }

      shader.color.r = Ignition::IO::ReadFloat();
      shader.color.g = Ignition::IO::ReadFloat();
      shader.color.b = Ignition::IO::ReadFloat();
      shader.color.a = Ignition::IO::ReadFloat();

      shader.intensity = Ignition::IO::ReadFloat();

      std::string s = Ignition::IO::ReadString();
      shader.albedo = Texture(); 
      shader.albedo.SetFlags(TextureFlags::Repeat | TextureFlags::Nearest);
      shader.albedo.LoadData((unsigned char*)grid_texture, 8, 8, 3, "Ignition_Grid");              
      
   }
}
