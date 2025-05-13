@echo off
set "FILE_NAME=klippad_sunrise_2_1k.hdr"

rem Sky
cmft --input %FILE_NAME% --filter none --srcFaceSize 256 --dstFaceSize 256 --edgeFixup none --outputNum 1 --output0 Sky --output0params ktx,rgba32f,cubemap

rem Radiance
cmft --input %FILE_NAME% --filter radiance --srcFaceSize 256 --dstFaceSize 256 --mipCount 7 --glossScale 10 --glossBias 3 --lightingModel blinnbrdf --edgeFixup none --outputNum 1 --output0 Rad --output0params ktx,rgba32f,cubemap

rem Irradiance
cmft --input %FILE_NAME% --filter irradiance --srcFaceSize 256 --dstFaceSize 256 --edgeFixup none --outputNum 1 --output0 Irr --output0params ktx,rgba32f,cubemap

pause
