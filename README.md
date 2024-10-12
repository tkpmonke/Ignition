# Ignition
Fast, Lightweight, and easy-to-use general purpose game engine

## Features
nothing for games yet, but the editor has a customizeable grid, can create objects, and thats it (for now)

## Compile From Source
<details>
    <summary>Details On Building</summary>

ignition uses the following

- GLEW
- GLFW
- GLM
- Assimp
- 7z (for hub)
- python (for hub)
- stb_image.h (included)
- IMGUI (included)

ignition runtime uses the following 

- lua
  
all of which can be installed using most linux package managers (only pacman/AUR tested) (excluding `stb_image.h` and `IMGUI`, but those are preinstalled and compiled by default
    
run `premake5 gmake2 && make` for unix-like systems. A optimization you can use is to pass the `-j` flag into make, running it with multiple threads (doesn't work on all systems)<br/><br/>
or<br/><br/>
run `premake5 vs2022` and open the .sln file for windows devices. Windows isn't fully supported yet, don't be surprised if it doesn't work (it doesn't)

</details>

## More Info
if your gonna make a game in this engine (when it's ready for real games to be made in it), then please feel free to contact me whenever if you have a question about the engine or would like to suggest a feature to be implemented :D

## Support

You can join [The Discord Server](https://discord.gg/ZPuBZ6VFVn) to support :D
