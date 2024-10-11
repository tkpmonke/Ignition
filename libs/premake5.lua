-- librarys not originally made by/for ignition have "ig-" added on to 
-- ensure they don't clash with system librarys

-- ImGui 
project "ig-imgui"
   kind           "StaticLib"
   language       "C++"
   cppdialect     "gnu++17"
   targetdir      "../bin"
   files      {   "imgui/**.cpp", "imgui/**.hpp" }
   includedirs{   "imgui" }
   links      {   "glfw", "GL" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

-- GLSL preprocessor
project "preglsl"
   kind           "StaticLib"
   language       "C++"
   cppdialect     "gnu++17"
   targetdir      "../bin"
   files      {   "preglsl/**.cpp", "preglsl/**.hpp" }
   includedirs{   "preglsl" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

