## File Name
- Shader file is named by its shader program and stage suffix.
  - e.g. `AAA_vert.glsl`, `AAA_frag.glsl`, `BBB_comp.glsl`
- Some files containing code shared with other shader files will become librarys with **no suffix**, which are placed in the `Lib` folder.
  - e.g. `CCC.glsl`

## Shared
To ensure consistency between CPU and GPU, we defined some macros such as **texture slot** and **uniform location** in `Shared.h` and include it in both cpp and shader side.

## Include
- `#include "..."` will look for header file in the current path, which is `"/SlamEngine/Engine/Asset/Shader"`.
- `#include <...>` will look for header file in a specific path, which is `"/SlamEngine/Engine/Asset/Shader/Lib"`.
- Make sure the included file contains **include guard**.

## Version
- The first line of shader must be `#version`.
