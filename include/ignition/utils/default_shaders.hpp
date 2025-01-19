#pragma once

#include <string>

const std::string unlit_vertex = 
"#version 450 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 normal;\n"
"layout(location = 2) in vec2 uv;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"out vec2 tex_uv;\n"
"out vec3 normals;\n"
"out vec3 frag_pos;\n"
"void main() {\n"
"  gl_Position = projection * model * vec4(position, 1);\n"
"  tex_uv = uv;\n"
"  normals = mat3(transpose(inverse(model))) * normal;\n"
"  frag_pos = vec3(model*vec4(position, 1));\n"
"}\0";

const std::string unlit_fragment = 
"#version 450 core\n"
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

const std::string no_texture_unlit_fragment = 
"#version 450 core\n"
"struct Material {\n"
"  vec4 color;\n"
"};\n"
"uniform Material material;\n"
"out vec4 o;\n"
"in vec2 tex_uv;\n"
"void main() {\n"
"  o = material.color;\n"
"}\0";

const std::string lit_fragment = 
"#version 450 core\n"

"struct Material {\n"
"  vec4 color;\n"
"  sampler2D albedo;\n"

"  int use_specular_map;\n"
"  sampler2D specular_map;\n"
"  vec3 specular;\n"

"  int use_normal_map;\n"
"  sampler2D normal_map;\n"

"  float shininess;\n"
"};\n"
"uniform Material material;\n"

"struct DirectionalLight {\n"
"  vec3 direction;\n"
"  vec3 ambient;\n"
"  vec3 diffuse;\n"
"  vec3 specular;\n"
"  sampler2D shadowMap;\n"
"};\n"
"uniform DirectionalLight dirLight;\n"

"struct Light {\n"
"  int isSpot;\n"
"  vec3 position;\n"
"  vec3 direction;\n"

"  float cutOff;\n"
"  float outerCutOff;\n"

"  float distance;\n"
"  float constant;\n"
"  float linear;\n"
"  float quadratic;\n"

"  vec3 ambient;\n"
"  vec3 diffuse;\n"
"  vec3 specular;\n"
"};\n"
"uniform int numberOfLights;\n"
"uniform Light lights[32];\n"

"in vec2 tex_uv;\n"

// function to calculate directional lighting
"vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {\n"
"  vec3 lightDir = normalize(-light.direction);\n"
"  float diff = max(dot(normal, lightDir), 0.0);\n"
"  vec3 reflectDir = reflect(-lightDir, normal);\n"
"  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
"  vec3 ambient = light.ambient * vec3(texture(material.albedo, tex_uv)*material.color);\n"
"  vec3 diffuse = light.diffuse * diff * vec3(texture(material.albedo, tex_uv)*material.color);\n"
"  vec3 specular = vec3(1);\n"
"  if (material.use_specular_map == 1) {\n"
"     specular = light.specular * spec * vec3(texture(material.specular_map, tex_uv));\n"
"  } else {\n"
"     specular = light.specular * spec * material.specular;\n"
"  }\n"
"  return (ambient+diffuse+specular);\n"
"}\n"

"vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {\n"
"  vec3 lightDir = normalize(light.position - fragPos);\n"
"  float diff = max(dot(normal, lightDir), 0.0);\n"
"  vec3 reflectDir = reflect(-lightDir, normal);\n"
"  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
"  float distance = length(light.position - fragPos);\n"
"  float attenuation = 1.0 / (light.constant + light.linear * distance +"
"                                            light.quadratic * (distance * distance));\n"
//"  float attenuation = 1;\n"
"  vec3 ambient  = light.ambient  * vec3(texture(material.albedo, tex_uv));\n"
"  vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.albedo, tex_uv));\n"

"  vec3 specular = vec3(1);\n"
"  if (material.use_specular_map == 1) {\n"
"     specular = light.specular * spec * vec3(texture(material.specular_map, tex_uv));\n"
"  } else {\n"
"     specular = light.specular * spec * material.specular;\n"
"  }\n"
"  ambient  *= attenuation;\n"
"  diffuse  *= attenuation;\n"
"  specular *= attenuation;\n"

"  return (ambient + diffuse + specular);\n"
"}\n"

"out vec4 o;\n"
"in vec3 normals;\n"
"in vec3 frag_pos;\n"
"uniform vec3 viewPos;\n"

"void main() {\n"
"  vec3 normal = vec3(1);\n"
"  if (material.use_normal_map == 1) {\n"
"     normal = normalize(texture(material.normal_map, tex_uv).rgb * 2.0 - 1.0);\n"
"  } else {\n"
"     normal = normalize(normals);\n"
"  }\n"
"  vec3 col = CalcDirLight(dirLight, normal, normalize(viewPos - frag_pos));\n"

"  for (int i = 0; i < numberOfLights; ++i)\n"
"     col += CalcPointLight(lights[i], normal, frag_pos, normalize(viewPos-frag_pos));\n"
//"     col += pointLights[i].diffuse;\n"

"  o = vec4(col, 1);\n"
"}\0";
