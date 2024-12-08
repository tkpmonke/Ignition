workspace "implosion"
   architecture "x86_64"
   configurations { "Debug", "Release"}


-- LIBRARYS (eg imgui) --
dofile("libs/premake5.lua")

project "ignition"
   kind "StaticLib"
   language "C++"
   cppdialect "c++17"
   targetdir "bin"
   includedirs { "include/ignition", "include/ignition/components", "include/ignition", "libs/assetpacker", "libs", "/usr/include/bullet" }
   files { "src/ignition/**.cpp", "include/ignition/**.hpp", "include/ignition/**.cpp", "include/ignition/**.h", "ig-jolt" }
   removefiles "hub/*"
   
   links { "glfw", "GL", "GLEW", "m", "assimp"}

   defines { "__INCLUDE_LUA_RUNTIME" }

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
   cppdialect "c++17"
   targetdir "bin"
   includedirs { "include/ignition", "include/ignition/components", "include/implosion", "include/implosion/utils", "libs/imgui", "libs/assetpacker", "libs", "/usr/include/bullet" }
   files { "src/implosion/**.cpp", "include/**.hpp", "libs/imgui/**.h" }
   removefiles "hub/*"
   links { "glfw", "GL", "GLEW", "m", "ignition", "assimp", "ig-imgui", "lua5.4", "ig-jolt"}

   defines { "EDITOR" }

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
   cppdialect "c++17"
   targetdir "bin"

   includedirs { "libs/imgui", "libs" }
   files {"utils/hub/hub.cpp", "libs/imgui/**.hpp"}
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
   cppdialect "c++17"
   targetdir "bin"
   includedirs { "include/ignition", "include/ignition/components", "include/runtime", "libs", "/usr/include/bullet" }
   files { "src/runtime/**.cpp", "include/runtime/**.hpp" }
   removefiles "hub/*"
   links { "glfw", "GL", "GLEW", "m", "ignition", "assimp", "lua5.4", "ig-jolt"}

   postbuildcommands { "sudo cp bin/ignition-runtime /usr/bin/" }
   
   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

