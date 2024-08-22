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

const std::string pp_bloom = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba8) uniform image2D scene;"
"layout(binding=1, rgba8) uniform image2D bright;"
//"layout(binding=2, rgba8) uniform image2D bloom;"
"uniform int passID;"
"const float kernel[3] = float[3](0.227027, 0.316216, 0.070270);"

"void pass1() {"
"   ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);"
"   vec4 sceneColor = imageLoad(scene, pixelCoords);"
"   if (sceneColor.r >= 0.7 || sceneColor.g >= 0.7 || sceneColor.b >= 0.7)"
"       imageStore(bright, pixelCoords, sceneColor);"
"   else"
"       imageStore(bright, pixelCoords, vec4(0));"
"}"
"void pass2() {"
"   ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);"
"   vec4 color = vec4(0);"
"   for (int x = -25; x <= 25; ++x) {"
"       for (int y = -25; y <= 25; ++y) {"
"           color.rgb += imageLoad(bright, pixelCoords + ivec2(x, y)).rgb;"
"       }"
"   }"
"   color.a = (color.r + color.g + color.b) / 150.0;"
"   imageStore(scene, pixelCoords, mix(color, imageLoad(scene,pixelCoords), 0.8));"
//"   imageStore(scene, pixelCoords, color);"
"}"
"void main() {"
"  if (passID == 0)"
"     pass1();"
"  else if (passID == 1)"
"     pass2();"
"}\0";

const std::string pp_tonemap = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba8) uniform image2D img;"
"const float gamma = 2.2;"
"const float exposure = 5;"
"const int type = 1;"
//0 = none
//1 = reinhard
//2 = exposure
//3 = aces
"void main() {"
"  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);"
"  vec4 color = imageLoad(img, texelCoord);"
"  if (type == 1) {"
"     vec4 mapped = color / (color+vec4(1));"
"     mapped = pow(mapped, vec4(1 / gamma));"
"  }"
"	imageStore(img, texelCoord, color);"
"}\0";
