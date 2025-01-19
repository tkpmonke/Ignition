#include "modules/rendering/meshrenderer.hpp"
#include "utils/files.hpp"
#include "shapes/cube.hpp"
#include "shapes/square.hpp"
#include "shapes/ico_sphere.hpp"
#include "utils/model_loader.hpp"
#include "utils/default_shaders.hpp"
#include "textures/grid.hpp"
#include "utils/io.hpp"
#include "modules/rendering/light.hpp"
#include "camera.hpp"

namespace Ignition::Rendering {
   int currentProgram = std::numeric_limits<int>::max();
   int currentVao = std::numeric_limits<int>::max();
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

      if (/*this->model.vertices.size() % 3 == 0 &&
          this->model.normals.size() % 3 == 0*/ true) {
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

   Camera* camera;

   void MeshRenderer::Start() {
      camera = Ignition::mainCamera;
   }
   
   void MeshRenderer::Update() {
      if (this->shader.program != (uint)currentProgram) {
         glUseProgram(this->shader.program);
         currentProgram = this->shader.program;
        
         if (camera->i_viewProj == Matrix4(0)) {
            camera->i_viewProj = camera->ViewProjectionMatrix();
         }

         this->shader.SetMatrix4(camera->i_viewProj, "projection");
      }
      if (this->vao != (uint)currentVao) {
         glBindVertexArray(this->vao); 
         currentVao = this->vao;
      }


      Matrix4 model = Matrix4(1.f);

      model = glm::translate(model, this->transform->position);
      model *= glm::mat4_cast(glm::quat(glm::radians(this->transform->rotation)));
      model = glm::scale(model, this->transform->scale);

      this->shader.SetMatrix4(model, "model");

      if (this->shader.type != ShaderType_None) {
         if (shader.albedo.type == IGNITION_2D) {

            glBindSampler(0, shader.albedo.sampler);
            glActiveTexture(GL_TEXTURE0 ); 
            glBindTexture(GL_TEXTURE_2D, this->shader.albedo);
            this->shader.SetInt(0, "material.albedo");
         }
      }
      this->shader.SetVec4(this->shader.color*this->shader.intensity, "material.color");
      if (this->shader.type == ShaderType_Lit)
      {
         glBindSampler(0, shader.albedo.sampler);
         this->shader.SetFloat(32, "material.shininess");
         this->shader.SetBool(false, "material.use_specular_map");
         this->shader.SetVec3(Vector3(0.5f), "material.specular");
         this->shader.SetBool(false, "material.use_normal_map");
         //glActiveTexture(GL_TEXTURE1); 
         //glBindTexture(GL_TEXTURE_2D, this->shader.diffuse);
         
         this->shader.SetVec3(directionalLight.direction, "dirLight.direction");
         this->shader.SetVec3(directionalLight.ambient, "dirLight.ambient");
         this->shader.SetVec3(directionalLight.diffuse, "dirLight.diffuse");
         this->shader.SetVec3(directionalLight.specular, "dirLight.specular");

         this->shader.SetInt(directionalLight.shadowMap.depth, "dirLight.shadowMap");

         glActiveTexture(GL_TEXTURE1);
         glBindTexture(GL_TEXTURE_2D, directionalLight.shadowMap.depth);
         this->shader.SetVec3(camera->transform.position, "viewPos");

         this->shader.SetInt((int)lights.size(), "numberOfLights");
         for (int l = 0; l < (int)lights.size(); ++l) {
            std::string varNameStart = "lights[" + std::to_string(l) + "].";

            this->shader.SetBool(lights[l]->type == LightType::Spot, varNameStart+"isSpot");

            this->shader.SetVec3(lights[l]->transform->position, varNameStart+"position");
            this->shader.SetVec3(lights[l]->ambient, varNameStart+"ambient");
            this->shader.SetVec3(lights[l]->diffuse, varNameStart+"diffuse");
            this->shader.SetVec3(lights[l]->specular, varNameStart+"specular");

            this->shader.SetFloat(lights[l]->distance, varNameStart+"distance");
            this->shader.SetFloat(lights[l]->constant, varNameStart+"constant");
            this->shader.SetFloat(lights[l]->linear, varNameStart+"linear");
            this->shader.SetFloat(lights[l]->quadratic, varNameStart+"quadratic");
            
            this->shader.SetFloat(lights[l]->cutOff, varNameStart+"cutOff");
            this->shader.SetFloat(lights[l]->outerCutOff, varNameStart+"outerCutOff");
         }
      }

      glDrawElements(GL_TRIANGLES, this->model.indices.size(), GL_UNSIGNED_INT, 0);
   }

   void MeshRenderer::Serialize() {
      Ignition::IO::WriteString(model.path);

      Ignition::IO::Write8((int)shader.type);
      Ignition::IO::WriteFloat(shader.color.r);
      Ignition::IO::WriteFloat(shader.color.g);
      Ignition::IO::WriteFloat(shader.color.b);
      Ignition::IO::WriteFloat(shader.color.a);

      Ignition::IO::WriteFloat(shader.intensity);

      Ignition::IO::WriteString(
            shader.albedo.name);
      Ignition::IO::Write8(shader.albedo.flags);
   }

   void MeshRenderer::Deserialize() {
      std::string modelName = Ignition::IO::ReadString();
      if (modelName == "cube")
         LoadModel(cube_model);
      else if (modelName == "square")
         LoadModel(square_model);
      else if (modelName == "ico_sphere")
         LoadModel(ico_sphere);
      else 
         LoadModel(Ignition::ModelLoader::LoadModel(Ignition::IO::GetProjectHome() + modelName));

      int type = Ignition::IO::Read8(); 
      switch(type) {
         case(ShaderType_Unlit): {
            Shader s = Shader(unlit_vertex, unlit_fragment, type);
            LoadShader(s);
            break;
         }
         case(ShaderType_Lit): {
            Shader s = Shader(unlit_vertex, lit_fragment, type);
            LoadShader(s);
            break;
         }
         case(ShaderType_Compute): {
            Shader s = Shader(unlit_vertex, type);
            LoadShader(s);
            break;
         }
      }
      
      shader.color.r = Ignition::IO::ReadFloat();
      shader.color.g = Ignition::IO::ReadFloat();
      shader.color.b = Ignition::IO::ReadFloat();
      shader.color.a = Ignition::IO::ReadFloat();

      shader.intensity = Ignition::IO::ReadFloat();

      std::string s = Ignition::IO::ReadString();
      shader.albedo = Texture(); 
      shader.albedo.SetFlags(Ignition::IO::Read8());
      if (s == "Ignition_Grid")
         shader.albedo.LoadData((unsigned char*)grid_texture, GRID_TEXTURE_SIZE_X, GRID_TEXTURE_SIZE_Y, GRID_TEXTURE_BPP, s);              
      else 
         shader.albedo.LoadData(s);
      
   }
}
