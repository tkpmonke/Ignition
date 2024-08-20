#include "gui/gui.hpp"
#include "types/shader.hpp"

#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <iostream>

using namespace Ignition::Rendering;

const char* gridVertexShader = 
"#version 330\n"
"layout (location = 0) in vec2 pos;"
"layout (location = 1) in float alphaMul;"
"uniform float height;"
"uniform mat4 proj;"
"out vec3 opos;"
"out float alpha;"
"void main() {"
"  gl_Position = proj * vec4(pos.x, height, pos.y, 1);"
"  opos = vec3(pos.x, 0, pos.y);"
"  alpha = alphaMul;"
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
"in float alpha;"
"float expo(float dis, float k, float b) {"
"  return pow(pow(b,k),dis-1);"
"}"
"void main() {"
"  float a = color.a;"
"  if (enableDistanceFalloff == 1) {"
"     a = -expo(distance(vec3(camera.x, opos.y, camera.z), opos)/dist, 5.7, distanceFalloff)+1*color.a;"
"  }"
"  if (a <= -0.5f)"
"     discard;"
"  o = vec4(color.rgb, a*alpha);"
"}";

const char* gridXYVertexShader = 
"#version 330\n"
"layout (location = 0) in vec2 pos;"
"layout (location = 1) in vec3 col;"
"uniform float height;"
"uniform mat4 proj;"
"out vec3 opos;"
"out vec3 ocol;"
"void main() {"
"  gl_Position = proj * vec4(pos.x, height+0.001f, pos.y, 1);"
"  opos = vec3(pos.x, 0, pos.y);"
"  ocol = col;"
"}";

const char* gridXYFragmentShader =
"#version 330\n"
"uniform vec3 camera;"
"uniform vec4 color;"
"uniform float dist;"
"uniform float distanceFalloff;"
"uniform int enableDistanceFalloff;"
"out vec4 o;"
"in vec3 opos;"
"in vec3 ocol;"
"float expo(float dis, float k, float b) {"
"  return pow(pow(b,k),dis-1);"
"}"
"void main() {"
"  float a = color.a;"
"  if (enableDistanceFalloff == 1) {"
"     a = -expo(distance(vec3(camera.x, opos.y, camera.z), opos)/(dist*1.5f), 5.7, distanceFalloff)+1*color.a;"
"  }"
"  if (a <= -0.5f)"
"     discard;"
"  o = vec4(mix(ocol, color.rgb,0.15f), a);"
"}";

namespace Implosion {
   void GUI::InitGrid()
   {
      Shader s = Shader(gridVertexShader, gridFragmentShader, ShaderType::Unlit);
      this->gridProgram = s.program;
      int gridCount = static_cast<int>(this->gridSize / this->gridSpacing );
      std::vector<float> vertices;
      float a;
      for (int i = -gridCount; i <= gridCount; ++i)
      {
         if (i != 0) {
            
            a = i % 5  == 0 ? 0.75f : 0.5f;
            a = i % 10 == 0 ? 1 : a;

            vertices.push_back(i * this->gridSpacing );
            vertices.push_back(this->gridSize ); 
            vertices.push_back(a); 
            vertices.push_back(i * this->gridSpacing );
            vertices.push_back(-this->gridSize ); 
            vertices.push_back(a); 

            vertices.push_back(-this->gridSize ); 
            vertices.push_back(i * this->gridSpacing );
            vertices.push_back(a); 
            vertices.push_back(this->gridSize ); 
            vertices.push_back(i * this->gridSpacing );
            vertices.push_back(a); 
         }
         
      }

      glGenVertexArrays(1, &this->gridVao);
      glGenBuffers(1, &this->gridVbo);
      glBindVertexArray(this->gridVao);

      glBindBuffer(GL_ARRAY_BUFFER, this->gridVbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2*sizeof(float)));
      glEnableVertexAttribArray(1);

      this->gridVertSize = vertices.size();

      std::vector<float> xyVertices;

      xyVertices.push_back(gridCount);
      xyVertices.push_back(0);
      xyVertices.push_back(1.0f);
      xyVertices.push_back(0);
      xyVertices.push_back(0);

      xyVertices.push_back(-gridCount);
      xyVertices.push_back(0);
      xyVertices.push_back(1.0f);
      xyVertices.push_back(0);
      xyVertices.push_back(0);

      xyVertices.push_back(0);
      xyVertices.push_back(gridCount);
      xyVertices.push_back(0);
      xyVertices.push_back(0);
      xyVertices.push_back(1.0f);

      xyVertices.push_back(0);
      xyVertices.push_back(-gridCount);
      xyVertices.push_back(0);
      xyVertices.push_back(0);
      xyVertices.push_back(1.0f);

      s = Shader(gridXYVertexShader, gridXYFragmentShader, ShaderType::Unlit);
      this->gridXYProgram = s.program;

      glGenVertexArrays(1, &this->gridXYVao);
      glGenBuffers(1, &this->gridXYVbo);
      glBindVertexArray(this->gridXYVao);

      glBindBuffer(GL_ARRAY_BUFFER, this->gridXYVbo);
      glBufferData(GL_ARRAY_BUFFER, xyVertices.size() * sizeof(float), &xyVertices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));
      glEnableVertexAttribArray(1);

      this->gridVertSize += xyVertices.size();
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
      glLineWidth(this->gridWidth);
      glBindVertexArray(this->gridVao);
      glDrawArrays(GL_LINES, 0, gridVertSize); 

      glUseProgram(this->gridXYProgram);
      glUniform1f(glGetUniformLocation(this->gridXYProgram, "height"), this->gridHeight);
      glUniform1f(glGetUniformLocation(this->gridXYProgram, "dist"), this->gridDistance);
      glUniform1f(glGetUniformLocation(this->gridXYProgram, "distanceFalloff"), this->gridFalloff);
      glUniform1i(glGetUniformLocation(this->gridXYProgram, "enableDistanceFalloff"), this->enableDistanceFalloff);
      glUniform4f(glGetUniformLocation(this->gridXYProgram, "color"), this->gridColor.x, this->gridColor.y, this->gridColor.z, this->gridColor.w);
      glUniform3f(glGetUniformLocation(this->gridXYProgram, "camera"), this->camera->transform.position.x, this->camera->transform.position.y, this->camera->transform.position.z);
      glUniformMatrix4fv(glGetUniformLocation(this->gridXYProgram, "proj"), 1, GL_FALSE, glm::value_ptr(this->camera->view_projection()));
      glLineWidth(this->gridWidth*1.25f);
      glBindVertexArray(this->gridXYVao);
      glDrawArrays(GL_LINES, 0, 4);
   }

   void GUI::RemakeGrid()
   {
      int gridCount = static_cast<int>(this->gridSize / this->gridSpacing );
      std::vector<float> vertices;
      for (int i = -gridCount; i <= gridCount; ++i)
      {
         vertices.push_back(i * this->gridSpacing + 0.5f);
         vertices.push_back(this->gridSize + 0.5f); 
         vertices.push_back(i * this->gridSpacing + 0.5f);
         vertices.push_back(-this->gridSize + 0.5f); 

         vertices.push_back(-this->gridSize + 0.5f); 
         vertices.push_back(i * this->gridSpacing + 0.5f);
         vertices.push_back(this->gridSize + 0.5f); 
         vertices.push_back(i * this->gridSpacing + 0.5f);
      }
      glBindBuffer(GL_ARRAY_BUFFER, this->gridVbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
      this->gridVertSize = vertices.size();

   }
}
