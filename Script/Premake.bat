@echo off

cd "%~dp0Premake"
"Windows\premake5.exe" vs2022

if "%1" neq "continue" (
    pause
)
