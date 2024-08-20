#include <string>

const std::string pp_invert = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba8) uniform image2D img;"
"void main() {"
"  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);"
"  vec4 color = imageLoad(img, texelCoord);"
"	imageStore(img, texelCoord, 1-color);"
"}\0";

const std::string pp_vignette = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba8) uniform image2D img;"
"void main() {"
"   ivec2 coord = ivec2(gl_GlobalInvocationID.xy);"
"   vec4 color = imageLoad(img, coord);"
"   vec2 uv = (gl_GlobalInvocationID.xy / vec2(imageSize(img)));"
"   vec2 center = vec2(0.5, 0.5);"
"   float dist = distance(uv, center);"
"   float vignette = smoothstep(1, 1 - 1, dist);"
"   vec4 newColor = color * vignette;"
"   imageStore(img, coord, newColor);"
"}\0";
