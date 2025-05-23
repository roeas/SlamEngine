@echo off

set "SCRIPT_PATH=%~dp0Script"
echo Script path: %SCRIPT_PATH%
echo.

rem Slam Engine
cd %SCRIPT_PATH%
"Premake\Windows\premake5.exe" vs2022

pause
