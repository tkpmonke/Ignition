#pragma once

#include <string>

const std::string unlit_vertex = 
"#version 330\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 normal;\n"
"layout(location = 2) in vec2 uv;\n"
"uniform mat4 projection;"
"uniform mat4 model;"
"out vec2 tex_uv;\n"
"void main() {\n"
"  gl_Position = projection * model * vec4(position, 1);\n"
"  tex_uv = uv;\n"
"}\0";

const std::string unlit_fragment = 
"#version 330\n"
"struct Material {\n"
"  vec4 color;\n"
"  sampler2D albedo;\n"
"};\n"
"uniform Material material;\n"
"out vec4 o;\n"
"in vec2 tex_uv;\n"
"void main() {\n"
"  o = material.color * texture(material.albedo, tex_uv);\n"
"}\0";

const std::string lit_fragment = 
"#version 330\n"
"struct Material {\n"
"  vec4 color;\n"
"  sampler2D albedo;\n"
"  sampler2D diffuse;\n"
"  float shininess;\n"
"  vec3 specular;\n"
"};\n"
"uniform Material material;\n" 
"out vec4 o;\n"
"void main() {\n"
"  o = color;\n"
"}\0";
