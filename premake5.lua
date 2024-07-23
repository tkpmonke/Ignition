workspace "overload"
   architecture "x86_64"
   configurations { "Debug", "Release"}

project "overload"
   kind "ConsoleApp"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"
   includedirs { "include/overload", "include/offload", "include/offload/imgui" }
   files { "**.h", "**.cpp", "**.hpp" }
   
   links { "glfw", "GL", "GLEW", "m" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

