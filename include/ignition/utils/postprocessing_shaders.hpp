#include <string>

const std::string pp_invert = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba32f) uniform image2D img;"
"void main() {"
"  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);"
"  vec4 color = imageLoad(img, texelCoord);"
"	imageStore(img, texelCoord, 1-color);"
"}\0";

const std::string pp_vignette = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba32f) uniform image2D img;"
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

const std::string pp_grayscale = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba32f) uniform image2D img;"
"void main() {"
"  ivec2 uv = ivec2(gl_GlobalInvocationID.xy);"
"  vec4 color = imageLoad(img, uv);"
"  float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));"
"  vec4 o = vec4(brightness, brightness, brightness, 1);"
"  imageStore(img, uv, o);"
"}\0";
const std::string pp_bloom = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba32f) uniform image2D scene;"
"layout(binding=1, rgba32f) uniform image2D bright;"
"layout(binding=2, rgba32f) uniform image2D bloom;"
"uniform int passID;"
"const float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);"
"const float threshold = 1;"

"void pass1() {"
"  ivec2 uv = ivec2(gl_GlobalInvocationID.xy);"
"  vec4 color = imageLoad(scene, uv);"
"  float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));"
"  if (brightness > threshold) {"
"     vec4 o = vec4(color.rgb, 1);"
"     imageStore(bright, uv, o);"
"  } else { imageStore(bright, uv, vec4(0,0,0,1)); }"
"}"

"void pass2(int m) {"   
"  ivec2 uv = ivec2(gl_GlobalInvocationID.xy);"
"  vec3 color = imageLoad(bright, uv).rgb * weight[0];"
"  if (m % 2 == 0) {"
"     for (int i = 1; i < 5; ++i) {"
"        color += imageLoad(bright, uv + ivec2(i, 0)).rgb * weight[i];"
"        color += imageLoad(bright, uv - ivec2(i, 0)).rgb * weight[i];"
"     }"
"  } else {"
"     for (int i = 1; i < 5; ++i) {"
"        color += imageLoad(bright, uv + ivec2(0, i)).rgb * weight[i];"
"        color += imageLoad(bright, uv - ivec2(0, i)).rgb * weight[i];"
"     }"
"  }"
"  imageStore(bloom, uv, vec4(color, 1));"
"}"

"void pass3() {"
"  ivec2 uv = ivec2(gl_GlobalInvocationID.xy);"
"  imageStore(scene, uv, imageLoad(scene, uv)+imageLoad(bloom, uv));"
"}"

"void main() {"
// getting brightest pixels
"  if (passID == 0)"
"     pass1();"
// making the bloom texture
"  if (passID == 1) {"
"     for (int i = 0; i < 20; ++i) pass2(i); }"
// applying the bloom texture
"  else if (passID == 2)"
"     pass3();"
"}\0";

const std::string pp_tonemap = 
"#version 430\n"
"layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;"
"layout(binding=0, rgba32f) uniform image2D img;"
"const float gamma = 2.2;"
"const float exposure = 10;"
"const int type = 1;"
//0 = none
//1 = reinhard
//2 = exposure
//3 = aces
"void main() {"
"  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);"
"  vec4 color = imageLoad(img, texelCoord);"
"  vec3 mapped = vec3(1) - exp(-color.rgb * exposure);"
"  mapped = pow(mapped, vec3(1 / gamma));"
"	imageStore(img, texelCoord, color);"
"}\0";
