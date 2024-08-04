workspace "implosion"
   architecture "x86_64"
   configurations { "Debug", "Release"}

project "implosion"
   kind "ConsoleApp"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"
   removefiles "hub/*"
   includedirs { "include/ignition", "include/ignition/components", "include/implosion", "imgui" }
   files { "**.h", "**.cpp", "**.hpp" }
   
   links { "glfw", "GL", "GLEW", "m" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "implosion"
   kind "ConsoleApp"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"

   includedirs { "imgui" }
   files { "hub/**.h", "hub/**.cpp", "hub/**.hpp" }
   
   links { "glfw", "GL" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

