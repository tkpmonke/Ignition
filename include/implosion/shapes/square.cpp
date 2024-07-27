#include "square.hpp"

Ignition::Model square_model {
   .indices = {0,1,3,1,2,3},
   .vertices = {1,1,0,1,-1,0,-1,-1,0,-1,1,0},
   .uv = {1,1,1,0,0,0,0,1}
};
