@echo off

set "ROOT_PATH=%~dp0"
set "THIRD_PARTY_PATH=%ROOT_PATH%Engine\Source\ThirdParty"
echo Root path: %ROOT_PATH%
echo ThirdParty path: %THIRD_PARTY_PATH%

rem Using vswhere to find MSBuild
set "VSWHERE_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`"%VSWHERE_PATH%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
  set "MSBUILD_PATH=%%i"
)
if defined MSBUILD_PATH (
    echo MSBuild found at: %MSBUILD_PATH%
) else (
    echo MSBuild not found. Please ensure Visual Studio is installed.
    exit /b 1
)
echo.

rem spdlog
set "SPDLOG_PATH=%THIRD_PARTY_PATH%\spdlog"
echo [ spdlog ] path: %SPDLOG_PATH%
cd %SPDLOG_PATH%
cmake -B build -DSPDLOG_USE_STD_FORMAT=ON
cmake --build build --target spdlog --config Debug
cmake --build build --target spdlog --config Release

pause
