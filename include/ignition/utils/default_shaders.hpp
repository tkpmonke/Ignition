#pragma once

#include <string>

const std::string unlit_vertex = R"(
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
uniform mat4 projection;
uniform mat4 model;
out vec2 tex_uv;
out vec3 normals;
out vec3 frag_pos;
void main() {
  gl_Position = projection * model * vec4(position, 1);
  tex_uv = uv;
  normals = mat3(transpose(inverse(model))) * normal;
  frag_pos = vec3(model*vec4(position, 1));
})";

const std::string unlit_fragment = R"(
#version 450 core
struct Material {
  vec4 color;
  sampler2D albedo;
};
uniform Material material;
out vec4 o;
in vec2 tex_uv;
void main() {
  o = material.color * texture(material.albedo, tex_uv);
})";

const std::string no_texture_unlit_fragment = R"(
#version 450 core
struct Material {
  vec4 color;
};
uniform Material material;
out vec4 o;
in vec2 tex_uv;
void main() {
  o = material.color;
})";

const std::string lit_fragment = R"(
#version 450 core

struct Material {
  vec4 color;
  sampler2D albedo;

  int use_specular_map;
  sampler2D specular_map;
  vec3 specular;

  int use_normal_map;
  sampler2D normal_map;

  float shininess;
};
uniform Material material;

struct DirectionalLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  sampler2D shadowMap;
};
uniform DirectionalLight dirLight;

struct Light {
  bool isSpot;
  vec3 position;
  vec3 direction;

  float cutOff;
  float outerCutOff;

  float power;
  float fallOff;
  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform int numberOfLights;
uniform Light lights[32];

in vec2 tex_uv;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 ambient = light.ambient * vec3(texture(material.albedo, tex_uv)*material.color);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.albedo, tex_uv)*material.color);
  vec3 specular = vec3(1);
  if (material.use_specular_map == 1) {
     specular = light.specular * spec * vec3(texture(material.specular_map, tex_uv));
  } else {
     specular = light.specular * spec * material.specular;
  }
  return (ambient+diffuse+specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.fallOff * pow(distance, 1/light.power));
  vec3 ambient  = light.ambient  * vec3(texture(material.albedo, tex_uv));
  vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.albedo, tex_uv));

  vec3 specular = vec3(1);
  if (material.use_specular_map == 1) {
     specular = light.specular * spec * vec3(texture(material.specular_map, tex_uv));
  } else {
     specular = light.specular * spec * material.specular;
  }
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.fallOff * pow(distance, 1/light.power));
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 ambient = light.ambient * vec3(texture(material.albedo, tex_uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.albedo, tex_uv));
    vec3 specular = vec3(1);
    if (material.use_specular_map == 1) {
       specular = light.specular * spec * vec3(texture(material.specular_map, tex_uv));
    } else {
       specular = light.specular * spec * material.specular;
    }

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

out vec4 o;
in vec3 normals;
in vec3 frag_pos;
uniform vec3 viewPos;

void main() {
  vec3 normal = vec3(1);
  if (material.use_normal_map == 1) {
     normal = normalize(texture(material.normal_map, tex_uv).rgb * 2.0 - 1.0);
  } else {
     normal = normalize(normals);
  }
  vec3 col = CalcDirLight(dirLight, normal, normalize(viewPos - frag_pos));

  for (int i = 0; i < numberOfLights; ++i)
      if (lights[i].isSpot)
         col += CalcSpotLight(lights[i], normal, frag_pos, normalize(viewPos-frag_pos));
      else
         col += CalcPointLight(lights[i], normal, frag_pos, normalize(viewPos-frag_pos));

  o = vec4(col, 1);
})";
