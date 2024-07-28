#pragma once

#include <string>

const std::string unlit_vertex = 
"#version 330\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 normal;\n"
"layout(location = 2) in vec2 uv;\n"
"uniform mat4 projection;"
"uniform mat4 model;"
"void main() {\n"
"  gl_Position = projection * model * vec4(position, 1);\n"
"}";

const std::string unlit_fragment = 
"#version 330\n"
"uniform vec4 color;\n"
"out vec4 o;\n"
"void main() {\n"
"  o = color;\n"
"}";
