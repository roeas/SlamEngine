# SlamEngine
[![MSBuild](https://github.com/roeas/SlamEngine/actions/workflows/MSBuild.yml/badge.svg?branch=main)](https://github.com/roeas/SlamEngine/actions/workflows/MSBuild.yml)

![Preview1](Engine/Asset/Texture/Preview1.png)
![Preview2](Engine/Asset/Texture/Preview2.png)

# Start
1. Install [CMake](https://cmake.org/download/#latest)
2. Install [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/)
5. Run `Setup.bat`

# Config diff
||Debug|Release|Final|
|:-|:-:|:-:|:-:|
|Macro|`SL_DEBUG`, `DEBUG`|`SL_RELEASE`, `NDEBUG`|`SL_FINAL`, `NDEBUG`|
|Symbol|On|On|Off|
|Optimization|Debug|On|Speed|
|CRT|/MDd|/MD|/MD|
|LTO|Off|Off|On|
|Assertion|On|On|Off|
|Break Point|On|On|On|
|Force Inlining|Off|Off|On|
|Shader Debug Info|On|Off|Off|

# Third party
[Premake](https://github.com/premake/premake-core) v5.0.0-beta7

[spdlog](https://github.com/gabime/spdlog) v1.15.3

[SDL](https://github.com/libsdl-org/SDL) v3.2.16

[glad](https://github.com/Dav1dde/glad) v2.0.8

[ImGui](https://github.com/ocornut/imgui) v1.91.9b

[ImPlot](https://github.com/epezent/implot) 3da8bd34299965d3b0ab124df743fe3e076fa222

[ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) 2310acda820d7383d4c4884b7945ada92cd16a47

[IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders) 14746043eda157a4c4e6773db1869e6aadd7c634

[GLM](https://github.com/g-truc/glm) v1.0.1

[Nameof](https://github.com/Neargye/nameof) v0.10.4

[magic_enum](https://github.com/Neargye/magic_enum) v0.9.7

[stb](https://github.com/nothings/stb) 013ac3beddff3dbffafd5177e7972067cd2b5083

[EnTT](https://github.com/skypjack/entt) v3.15.0

[Shaderc](https://github.com/google/shaderc) v2025.2

[Assimp](https://github.com/assimp/assimp) v5.4.3

[GLI](https://github.com/g-truc/gli) 779b99ac6656e4d30c3b24e96e0136a59649a869

[Tracy](https://github.com/wolfpld/tracy) v0.11.1

[cmft](https://github.com/dariomanesku/cmft) cad5f31bac66fd05987d667af62311c444df6d46

[Noto Sans](https://fonts.google.com/noto)

[Material Symbols](https://fonts.google.com/icons)
