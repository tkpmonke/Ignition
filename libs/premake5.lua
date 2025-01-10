-- librarys not originally made by/for ignition have "ig-" added on to 
-- ensure they don't clash with system librarys

-- ImGui 
project "ig-imgui"
   kind           "StaticLib"
   language       "C++"
   cppdialect     "c++17"
   targetdir      "../bin"
   files      {   "imgui/**.cpp", "imgui/**.hpp" }
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
   cppdialect     "c++17"
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

-- Jolt Physics
project "ig-jolt"
   kind           "StaticLib"
   language       "C++"
   cppdialect     "c++17"
   targetdir      "../bin"
   files      {   "Jolt/**.cpp", "Jolt/**.hpp" }
   includedirs { "./" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "ig-angelscript"
   kind           "StaticLib"
   language       "C++"
   cppdialect     "c++17"
   targetdir      "../bin"
   files      {   "angelscript/**.cpp", "angelscript/**.h" }
   includedirs { "./", "./angelscript", "./angelscript/include" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
