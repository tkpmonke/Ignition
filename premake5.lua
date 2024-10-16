workspace "implosion"
   architecture "x86_64"
   configurations { "Debug", "Release"}

-- LIBRARYS (eg imgui) --
dofile("libs/premake5.lua")

project "ignition"
   kind "StaticLib"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"
   includedirs { "include/ignition", "include/ignition/components", "include/ignition" }
   files { "src/ignition/**.cpp", "include/ignition/**.hpp", "include/ignition/**.cpp", "include/ignition/**.h" }
   removefiles "hub/*"
   
   links { "glfw", "GL", "GLEW", "m", "assimp"}

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
   includedirs { "include/ignition", "include/ignition/components", "include/implosion", "include/implosion/utils", "libs/imgui" }
   files { "src/implosion/**.cpp", "include/**.hpp", "libs/imgui/**.h" }
   removefiles "hub/*"
   links { "glfw", "GL", "GLEW", "m", "ignition", "assimp", "ig-imgui" }

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

   includedirs { "libs/imgui" }
   files { "hub/**.h", "hub/**.cpp", "hub/**.hpp", "libs/imgui/**.hpp"}
   links { "glfw", "GL", "ig-imgui" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "ignition-runtime"
   kind "ConsoleApp"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"
   includedirs { "include/ignition", "include/ignition/components", "include/runtime" }
   files { "src/runtime/**.cpp", "include/runtime/**.hpp" }
   removefiles "hub/*"
   links { "glfw", "GL", "GLEW", "m", "ignition", "assimp", "lua" }

   defines { "__INCLUDE_LUA_RUNTIME" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
