#include "gui/gui.hpp"
#include "types/shader.hpp"

#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <iostream>

using namespace Ignition::Rendering;

const char* gridVertexShader = 
"#version 330\n"
"layout (location = 0) in vec2 pos;"
"uniform float height;"
"uniform mat4 proj;"
"out vec3 opos;"
"void main() {"
"  gl_Position = proj * vec4(pos.x, height, pos.y, 1);"
"  opos = vec3(pos.x, 0, pos.y);"
"}";

const char* gridFragmentShader =
"#version 330\n"
"uniform vec4 color;"
"uniform vec3 camera;"
"uniform float dist;"
"uniform float distanceFalloff;"
"uniform int enableDistanceFalloff;"
"out vec4 o;"
"in vec3 opos;"
"float expo(float dis, float k, float b) {"
"  return pow(pow(b,k),dis-1);"
"}"
"void main() {"
"  float a = color.a;"
"  if (enableDistanceFalloff == 1) {"
"     a = -expo(distance(vec3(camera.x, opos.y, camera.z), opos)/dist, 5.7, distanceFalloff)+1*color.a;"
"  }"
"  o = vec4(color.rgb, a);"
"}";

namespace Implosion {
   void GUI::InitGrid()
   {
      Shader s = Shader(gridVertexShader, gridFragmentShader, ShaderType::Unlit);
      this->gridProgram = s.program;
      int gridCount = static_cast<int>(this->gridSize / this->gridSpacing );
      std::vector<float> vertices;
      for (int i = -gridCount; i <= gridCount; ++i)
      {
         vertices.push_back(i * this->gridSpacing);
         vertices.push_back(this->gridSize); 
         vertices.push_back(i * this->gridSpacing);
         vertices.push_back(-this->gridSize); 

         vertices.push_back(-this->gridSize); 
         vertices.push_back(i * this->gridSpacing);
         vertices.push_back(this->gridSize); 
         vertices.push_back(i * this->gridSpacing);
      }

      glGenVertexArrays(1, &this->gridVao);
      glGenBuffers(1, &this->gridVbo);
      glBindVertexArray(this->gridVao);

      glBindBuffer(GL_ARRAY_BUFFER, this->gridVbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      this->gridVertSize = vertices.size();
   }
   void GUI::RenderGrid()
   {
      if (!this->enableGrid)
         return;


      glUseProgram(this->gridProgram);
      glUniform1f(glGetUniformLocation(this->gridProgram, "height"), this->gridHeight);
      glUniform1f(glGetUniformLocation(this->gridProgram, "dist"), this->gridDistance);
      glUniform1f(glGetUniformLocation(this->gridProgram, "distanceFalloff"), this->gridFalloff);
      glUniform1i(glGetUniformLocation(this->gridProgram, "enableDistanceFalloff"), this->enableDistanceFalloff);
      glUniform4f(glGetUniformLocation(this->gridProgram, "color"), this->gridColor.x, this->gridColor.y, this->gridColor.z, this->gridColor.w);
      glUniform3f(glGetUniformLocation(this->gridProgram, "camera"), this->camera->transform.position.x, this->camera->transform.position.y, this->camera->transform.position.z);
      glUniformMatrix4fv(glGetUniformLocation(this->gridProgram, "proj"), 1, GL_FALSE, glm::value_ptr(this->camera->view_projection()));
      glBindVertexArray(this->gridVao);
      glDrawArrays(GL_LINES, 0, gridVertSize); 
   }

   void GUI::RemakeGrid()
   {
      int gridCount = static_cast<int>(this->gridSize / this->gridSpacing );
      std::vector<float> vertices;
      for (int i = -gridCount; i <= gridCount; ++i)
      {
         vertices.push_back(i * this->gridSpacing);
         vertices.push_back(this->gridSize); 
         vertices.push_back(i * this->gridSpacing);
         vertices.push_back(-this->gridSize); 

         vertices.push_back(-this->gridSize); 
         vertices.push_back(i * this->gridSpacing);
         vertices.push_back(this->gridSize); 
         vertices.push_back(i * this->gridSpacing);
      }
      glBindBuffer(GL_ARRAY_BUFFER, this->gridVbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
      this->gridVertSize = vertices.size();

   }
}
