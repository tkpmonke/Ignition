workspace "implosion"
   architecture "x86_64"
   configurations { "Debug", "Release"}

project "implosion"
   kind "ConsoleApp"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"
   includedirs { "include/ignition", "include/implosion", "include/implosion/imgui" }
   files { "**.h", "**.cpp", "**.hpp" }
   
   links { "glfw", "GL", "GLEW", "m" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

