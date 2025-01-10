workspace "implosion"
   architecture "x86_64"
   configurations { "Debug", "Release"}
   pic "On"
   
-- LIBRARYS (eg imgui) --
dofile("libs/premake5.lua")
dofile("generate-docs.lua")

project "ignition"
   kind "SharedLib"
   language "C++"
   cppdialect "c++17"
   targetdir "bin"
   includedirs { "include/ignition", "include/ignition/components", "include/ignition", "libs/assetpacker", "libs" }
   files { "src/ignition/**.cpp", "include/ignition/**.hpp", "include/ignition/**.cpp", "include/ignition/**.h" }
   removefiles "hub/*"
   
   links { "glfw", "GL", "GLEW", "m", "assimp", "lua5.4", "ig-jolt", "ig-angelscript"}

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
   links { "glfw", "GL", "GLEW", "ignition", "ig-imgui", "lua5.4"}

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

   -- a majority is written in c, but the files that interface with ignition are c++ due to
   -- ignition being a c++ library

   language "C++"
   cppdialect "c++17"
   cdialect "c99"
   targetdir "bin"

   links { "glfw", "GL", "GLEW", "ignition" }
   includedirs { "include/ignition", "libs", "libs/imgui" }
   files {"utils/hub/**.c", "utils/hub/**.cpp", "utils/hub/**.h", "include/**.hpp" }

   defines { "IGNITION_NO_ASSIMP" }

   filter "files:**.c"
      language "C"


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
   links { "glfw", "GL", "GLEW", "ignition", "lua5.4", "ig-angelscript" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"


project "install"
   kind "Utility"
   dependson { "ignition", "implosion", "ignition-runtime", "implosion-hub" }

   postbuildcommands {
      "sudo ./install.sh"
   }
