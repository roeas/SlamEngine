#include "Shared/ToneMapping.h"

vec3 Clamp(vec3 color)
{
    return clamp(color, vec3(0.0), vec3(1.0));
}

vec3 Reinhard(vec3 color)
{
    return color / (color + 1.0);
}

vec3 HableFilmicPartial(vec3 x)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;

    return ((x * (x * A + C * B) + D * E) / (x * (x * A + B) + D * F)) - E / F;
}

vec3 HableFilmic(vec3 color)
{
    float exposureBias = 2.0;
    vec3 curr = HableFilmicPartial(color * exposureBias);

    vec3 W = vec3(11.2);
    vec3 whiteScale = vec3(1.0) / HableFilmicPartial(W);

    return curr * whiteScale;
}

vec3 ACES(vec3 color)
{
    const mat3 inputFactor = mat3(
        0.59719, 0.07600, 0.02840,
        0.35458, 0.90834, 0.13383,
        0.04823, 0.01566, 0.83777);
    const mat3 outputFactor = mat3(
        1.60475, -0.10208, -0.00327,
        -0.53108, 1.10813, -0.07276,
        -0.07367, -0.00605, 1.07602);

    color = inputFactor * color;
    vec3 a = color * (color + 0.0245786) - 0.000090537;
    vec3 b = color * (color * 0.983729 + 0.4329510) + 0.238081;
    color = a / b;

    return outputFactor * color;
}

vec3 ACES_Simple(vec3 color)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    color *= 0.6;

    return (color * (color * a + b)) / (color * (color * c + d) + e);
}

vec3 PBRNeutral(vec3 color)
{
    const float startCompression = 0.8 - 0.04;
    const float desaturation = 0.15;
    
    float x = min(color.r, min(color.g, color.b));
    float offset = x < 0.08 ? x - 6.25 * x * x : 0.04;
    color -= offset;
    
    float peak = max(color.r, max(color.g, color.b));
    if (peak < startCompression)
    {
        return color;
    }
    
    const float d = 1.0 - startCompression;
    float newPeak = 1.0 - d * d / (peak + d - startCompression);
    color *= newPeak / peak;
    
    float g = 1.0 - 1.0 / (desaturation * (peak - newPeak) + 1.0);
    return mix(color, vec3(newPeak), g);
}
