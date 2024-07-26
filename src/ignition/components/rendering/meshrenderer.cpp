#include "components/rendering/meshrenderer.hpp"

namespace Ignition::Rendering {
   void MeshRenderer::LoadModel(Model m)
   {
      unsigned int VBO, EBO;
      glGenVertexArrays(1, &this->vao);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);
      glBindVertexArray(this->vao);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(float), &m.vertices[0], GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(int), &m.indices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);

      glBindBuffer(GL_ARRAY_BUFFER, 0); 

      glBindVertexArray(0); 
       
   }

   void MeshRenderer::Update() {
      glUseProgram(this->shader.program);
      glBindVertexArray(this->vao); 
      glDrawElements(GL_TRIANGLES, this->model.indices.size(), GL_UNSIGNED_INT, 0);
   }
}
