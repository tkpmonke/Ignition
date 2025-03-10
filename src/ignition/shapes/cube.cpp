#include "shapes/cube.hpp"

const Ignition::Model cube_model {
   .indices = {0,  3,  2,
               2,  1,  0,
               4,  5,  6,
               6,  7,  4,
               11, 8,  9,
               9,  10, 11,
               12, 13, 14,
               14, 15, 12,
               16, 17, 18,
               18, 19, 16,
               20, 21, 22,
               22, 23, 20},
   .vertices = {-0.5f, -0.5f, -0.5f, 
                  0.5f, -0.5f, -0.5f, 
                  0.5f,  0.5f, -0.5f, 
                  -0.5f,  0.5f, -0.5f,
                  -0.5f, -0.5f,  0.5f,
                  0.5f, -0.5f,  0.5f,
                  0.5f,  0.5f,  0.5f,
                  -0.5f,  0.5f,  0.5f,
 
                  -0.5f,  0.5f, -0.5f,
                  -0.5f, -0.5f, -0.5f,
                  -0.5f, -0.5f,  0.5f,
                  -0.5f,  0.5f,  0.5f,
                  0.5f, -0.5f, -0.5f,
                  0.5f,  0.5f, -0.5f,
                  0.5f,  0.5f,  0.5f,
                  0.5f, -0.5f,  0.5f,
 
                  -0.5f, -0.5f, -0.5f,
                  0.5f, -0.5f, -0.5f,
                  0.5f, -0.5f,  0.5f, 
                  -0.5f, -0.5f,  0.5f,
                  0.5f,  0.5f, -0.5f,
                  -0.5f,  0.5f, -0.5f,
                  -0.5f,  0.5f,  0.5f,
                  0.5f,  0.5f,  0.5f},
   .normals = {
      // Front face
      0.0f,  0.0f,  -1.0f,  // Vertex 4
      0.0f,  0.0f,  -1.0f,  // Vertex 5
      0.0f,  0.0f,  -1.0f,  // Vertex 6
      0.0f,  0.0f,  -1.0f,  // Vertex 7

      // Back face
      0.0f,  0.0f,  1.0f,  // Vertex 0
      0.0f,  0.0f,  1.0f,  // Vertex 1
      0.0f,  0.0f,  1.0f,  // Vertex 2
      0.0f,  0.0f,  1.0f,  // Vertex 3

      // Left face
   -  1.0f,  0.0f,  0.0f,  // Vertex 8
   -  1.0f,  0.0f,  0.0f,  // Vertex 9
   -  1.0f,  0.0f,  0.0f,  // Vertex 10
   -  1.0f,  0.0f,  0.0f,  // Vertex 11

      // Right face
      1.0f,  0.0f,  0.0f,  // Vertex 12
      1.0f,  0.0f,  0.0f,  // Vertex 13
      1.0f,  0.0f,  0.0f,  // Vertex 14
      1.0f,  0.0f,  0.0f,  // Vertex 15

      // Top face
      0.0f,  -1.0f,  0.0f,  // Vertex 20
      0.0f,  -1.0f,  0.0f,  // Vertex 21
      0.0f,  -1.0f,  0.0f,  // Vertex 22
      0.0f,  -1.0f,  0.0f,  // Vertex 23

      // Bottom face
      0.0f, 1.0f,  0.0f,  // Vertex 16
      0.0f, 1.0f,  0.0f,  // Vertex 17
      0.0f, 1.0f,  0.0f,  // Vertex 18
      0.0f, 1.0f,  0.0f,  // Vertex 19
   },
   .uv = { 0,0,0,
           1,0,0,
           1,1,0,
           0,1,0,
           0,0,0,
           1,0,0,
           1,1,0,
           0,1,0,

           0,0,0,
           1,0,0,
           1,1,0,
           0,1,0,
           0,0,0,
           1,0,0,
           1,1,0,
           0,1,0,
           
           0,0,0,
           1,0,0,
           1,1,0,
           0,1,0,
           0,0,0,
           1,0,0,
           1,1,0,
           0,1,0},
   .path = "cube",
   .name = "cube"
};
