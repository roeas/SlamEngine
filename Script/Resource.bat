@echo off

set "FILE_NAME=Skybox.hdr"

for %%A in ("%~dp0..") do set "ROOT_PATH=%%~fA"
set "CMFT_PATH=%ROOT_PATH%/Tool/cmft/cmft.exe"
cd "%ROOT_PATH%/Engine/Asset/Texture"

rem Sky
%CMFT_PATH% --input %FILE_NAME% --filter none --dstFaceSize 256 --edgeFixup none --outputNum 1 --output0 Sky --output0params ktx,rgba32f,cubemap

rem Radiance
%CMFT_PATH% --input %FILE_NAME% --filter radiance --dstFaceSize 256 --mipCount 7 --glossScale 10 --glossBias 3 --lightingModel blinnbrdf --edgeFixup none --outputNum 1 --output0 Rad --output0params ktx,rgba32f,cubemap

rem Irradiance
%CMFT_PATH% --input %FILE_NAME% --filter irradiance --dstFaceSize 256 --edgeFixup none --outputNum 1 --output0 Irr --output0params ktx,rgba32f,cubemap

if "%1" neq "continue" (
    pause
)
