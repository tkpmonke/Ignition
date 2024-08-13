workspace "implosion"
   architecture "x86_64"
   configurations { "Debug", "Release"}

project "implosion"
   kind "ConsoleApp"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"
   includedirs { "include/ignition", "include/ignition/components", "include/implosion", "include/implosion/utils", "imgui" }
   files { "src/**.cpp", "include/**.hpp", "include/**.cpp", "include/**.h", "imgui/**.h", "imgui/**.cpp" }
   removefiles "hub/*"
   
   links { "glfw", "GL", "GLEW", "m", "assimp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "implosion-hub"
   kind "ConsoleApp"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"

   includedirs { "imgui" }
   files { "hub/**.h", "hub/**.cpp", "hub/**.hpp", "imgui/**.cpp", "imgui/**.hpp"}
   
   links { "glfw", "GL" }
   objdir "obj/hub"

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

