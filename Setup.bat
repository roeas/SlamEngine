@echo off

set "ROOT_PATH=%~dp0"
set "SCRIPT_PATH=%ROOT_PATH%Script"
set "THIRD_PARTY_PATH=%ROOT_PATH%Engine\Source\ThirdParty\"
echo Root path: %ROOT_PATH%
echo Script path: %SCRIPT_PATH%
echo ThirdParty path: %THIRD_PARTY_PATH%

rem Find MSBuild by vswhere
set "VSWHERE_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`"%VSWHERE_PATH%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
  set "MSBUILD_PATH=%%i"
)
if defined MSBUILD_PATH (
    echo MSBuild found at: %MSBUILD_PATH%
) else (
    echo MSBuild not found.
)
echo.

set "CMAKE_GENERATOR=Visual Studio 17 2022"
set "CMAKE_ARCHITECTURE=x64"

rem spdlog
set "SPDLOG_PATH=%THIRD_PARTY_PATH%\spdlog"
echo [ spdlog ] path: %SPDLOG_PATH%
cd %SPDLOG_PATH%
cmake -G "%CMAKE_GENERATOR%" -A %CMAKE_ARCHITECTURE% -B build -DSPDLOG_USE_STD_FORMAT=ON -DSPDLOG_NO_EXCEPTIONS=ON
cmake --build build --target spdlog --config Debug
cmake --build build --target spdlog --config Release
echo.

rem SDL
set "SDL_PATH=%THIRD_PARTY_PATH%\sdl"
echo [ SDL ] path: %SDL_PATH%
cd %SDL_PATH%
cmake -G "%CMAKE_GENERATOR%" -A %CMAKE_ARCHITECTURE% -B build
cmake --build build --target SDL3-shared --config Debug
cmake --build build --target SDL3-shared --config Release
echo.

rem Shaderc
set "SHADERC_PATH=%THIRD_PARTY_PATH%\shaderc"
echo [ Shaderc ] path: %SHADERC_PATH%
cd %SHADERC_PATH%
python "utils\git-sync-deps"
cmake -G "%CMAKE_GENERATOR%" -A %CMAKE_ARCHITECTURE% -B build -DSHADERC_ENABLE_SHARED_CRT=ON
cmake --build build --target shaderc_combined --config Debug
cmake --build build --target shaderc_combined --config Release
echo.

rem assimp
set "ASSIMP_PATH=%THIRD_PARTY_PATH%\assimp"
echo [ Assimp ] path: %ASSIMP_PATH%
cd %ASSIMP_PATH%

cmake -G "%CMAKE_GENERATOR%" -A %CMAKE_ARCHITECTURE% -B build -DBUILD_SHARED_LIBS=OFF
cmake --build build --target assimp --config Debug
cmake --build build --target assimp --config Release
echo.

rem Tracy
set "TRACY_PATH=%THIRD_PARTY_PATH%\tracy"
echo [ Tracy ] path: %TRACY_PATH%
cd %TRACY_PATH%
cmake -G "%CMAKE_GENERATOR%" -A %CMAKE_ARCHITECTURE% -B build
cmake --build build --target TracyClient --config Debug
cmake --build build --target TracyClient --config Release
echo.

rem Slam Engine
cd %SCRIPT_PATH%
"Premake\Windows\premake5.exe" vs2022

pause
