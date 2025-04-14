@echo off

set "ROOT_PATH=%~dp0"
set "THIRD_PARTY_PATH=%ROOT_PATH%Engine\Source\ThirdParty\"
echo Root path: %ROOT_PATH%
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

rem SDL
set "SDL_PATH=%THIRD_PARTY_PATH%\sdl"
echo [ SDL ] path: %SDL_PATH%
cd %SDL_PATH%
cmake -B build
cmake --build build --target SDL3-shared --config Debug
cmake --build build --target SDL3-shared --config Release
echo.

pause
