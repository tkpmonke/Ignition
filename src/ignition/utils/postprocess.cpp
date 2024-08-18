#include "utils/postprocess.hpp"

const char* quadPostProcessV = 
"#version 430 core\n"
"layout(location=0) in vec3 pos;"
"layout(location=1) in vec2 uv;"
"out vec2 UV;"
"void main() {"
"  UV=uv;"
"  gl_Position = vec4(pos, 1);"
"}\0";

const char* quadPostProcessF =
"#version 430 core\n"
"out vec4 fragColor;"
"in vec2 UV;"
"uniform sampler2D tex;"
"void main() {"
"  fragColor = vec4(texture(tex, UV).rgb, 1);"
"}\0";

namespace Ignition::Rendering {
   PostProcess::PostProcess(std::string data, Window* window) 
      : window(window) 
   {

   }
}
