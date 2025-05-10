@echo off
set "FILE_NAME=klippad_sunrise_2_1k.hdr"

:: We assume the inputs are in sRGB space and ultimately stored in linear space.

rem sky
cmft --input %FILE_NAME% --filter none --srcFaceSize 256 --dstFaceSize 256 --edgeFixup none --inputGammaNumerator 1.0 --inputGammaDenominator 2.2 --outputGammaNumerator 1.0 --outputGammaDenominator 1.0 --outputNum 1 --output0 sky --output0params ktx,rgba32f,cubemap

rem radiance
cmft --input %FILE_NAME% --filter radiance --srcFaceSize 256 --dstFaceSize 256 --mipCount 7 --glossScale 10 --glossBias 3 --lightingModel blinnbrdf --edgeFixup none --inputGammaNumerator 1.0 --inputGammaDenominator 2.2 --outputGammaNumerator 1.0 --outputGammaDenominator 1.0 --outputNum 1 --output0 rad --output0params ktx,rgba32f,cubemap

rem irradiance
cmft --input %FILE_NAME% --filter irradiance --srcFaceSize 256 --dstFaceSize 256 --edgeFixup none --inputGammaNumerator 1.0 --inputGammaDenominator 2.2 --outputGammaNumerator 1.0 --outputGammaDenominator 1.0 --outputNum 1 --output0 irr --output0params ktx,rgba32f,cubemap

pause
