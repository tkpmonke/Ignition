# Ignition
Fast, Lightweight, and easy-to-use general purpose game engine

## Installation
Right now Ignition is only avalible on the AUR, with APT planned in the near future

```
# for arch based distros
# you can use your favorite pacman wrapper
yay -S implosion
```

## Compile From Source
<details>
    <summary>Details On Building</summary>

ignition uses the following

- GLEW
- GLFW
- GLM
- Assimp
- LuaBridge
- 7z (for hub)
- python (for hub)
- stb_image.h (included)
- IMGUI (included)

all of which can be installed using most linux package managers (only pacman/AUR tested)
    
run `premake5 gmake2 && make` for unix-like systems. A optimization you can use is to pass the `-j` flag into make, running it with multiple threads (doesn't work on all systems)<br/><br/>
or<br/><br/>
run `premake5 vs2022` and open the .sln file for windows devices. Windows isn't fully supported yet, don't be surprised if it doesn't work (it doesn't)

</details>

## Future plans
currently, the plans of this engine are undecided. Once I add lua, physics, audio, and AI pathfinding, I will most likely start working on real games using Ignition. Once that point happens, I will most likely only do bug fixes to the engine, and features that i feel like will benifit me in my games.

## More Info
if your gonna make a game in this engine (when it's ready for real games to be made in it), then please feel free to contact me whenever if you have a question about the engine or would like to suggest a feature to be implemented :D

## Support

You can join [The Discord Server](https://discord.gg/ZPuBZ6VFVn) to support :D
