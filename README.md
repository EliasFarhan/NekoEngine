# Neko Game Engine 

Neko is a 3D game engine based on SDL2 and OpenGL ES 3.2 that works on Desktop (Windows and Linux, not MacOSX), Android and Nintendo Switch used at [SAE Institute Geneva](https://sae.swiss).

## Requirements
### Windows
Install CMake ([https://cmake.org/download/]), vcpkg ([https://github.com/microsoft/vcpkg.git]), the Vulkan SDK ([https://www.lunarg.com/vulkan-sdk/]) and Python3 ([https://www.python.org/downloads/]). Visual Studio 2019 is recommended on Windows.
### Ubuntu
Because we use C++17 filesystem, you will need at least Ubuntu 19.10 to compile with the latest libstdc++ or even better Clang and libc++. You will need to install the packages:
```
sudo apt-get install build-essential cmake libsdl2-dev glslang-tools python3-dev libgl1-mesa-dev libzstd-dev 
```
Then you need to install vcpkg as well.
### Android
Simply use Android Studio and open the android/GPR5300 folder. You will need to install NDK. The Android Studio cmake version is too old so put your recent cmake into the PATH as well as Ninja ([https://ninja-build.org/]).
## Libraries used
- SDL2 [https://www.libsdl.org/index.php]
- GLEW [http://glew.sourceforge.net/]
- googletest [https://github.com/google/googletest]
- google benchmark [https://github.com/google/benchmark]
- imgui [https://github.com/ocornut/imgui]
- sole uuid [https://github.com/r-lyeh-archived/sole]
- xxhash [https://github.com/Cyan4973/xxHash]
- stb_image [https://github.com/nothings/stb]
- assimp [https://github.com/assimp/assimp]
- SFML net [https://www.sfml-dev.org/]
- freetype [https://www.freetype.org/]
- tracy [https://github.com/wolfpld/tracy.git]
- fmt [https://github.com/fmtlib/fmt]
- nlohmann's json [https://github.com/nlohmann/json]
- units [https://github.com/nholthaus/units]
- sqlite3 [https://www.sqlite.org/index.html]
- protobuf [https://developers.google.com/protocol-buffers]
## Credits
- Elias Farhan
- Fred Dubouchet
- Simon Canas
- Luca Floreau
- Guillaume Jeannin
- Stephen Grosjean
- Sébastien Feser