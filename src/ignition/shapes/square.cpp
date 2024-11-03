#include "shapes/square.hpp"

const Ignition::Model square_model {
   .indices = {0,1,3,
               1,2,3},
   .vertices = {.5f, 0, .5f, 
                .5f, 0,-.5f, 
               -.5f, 0,-.5f, 
               -.5f, 0, .5f, },
   .normals = {0,0,0,0,0,0,0,0,0,0,0,0},
   .uv = {1,1,0,
          1,0,0,
          0,0,0,
          0,1,0},
   .path = "square"
};
