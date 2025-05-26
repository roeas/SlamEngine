@echo off

set "SCRIPT_PATH=%~dp0Script"

call "%SCRIPT_PATH%\ThirdParty.bat" continue
call "%SCRIPT_PATH%\Premake.bat" continue
call "%SCRIPT_PATH%\Resource.bat" continue

pause
