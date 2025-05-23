#ifndef SL_BRDF_GLSL
#define SL_BRDF_GLSL

#include <Common.glsl>

float DistributionGGX(float roughness, float NdotH)
{
    // a = roughness^2
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    denom = SL_PI * denom * denom;

    return a2 / denom;
}

float GeometrySchlickSmith(float roughness, float NdotV, float NdotL)
{
    float r = (roughness + 1.0) * 0.5;
    float a = r * r;
    float denomV = NdotV * (2.0 - a) + a;
    float denomL = NdotL * (2.0 - a) + a;

    return (4.0 * NdotV * NdotL) / (denomV * denomL);
}

float VisibilitySchlickSmith(float roughness, float NdotV, float NdotL)
{
    // BRDF = (D * G * F) / (4 * NdotV * NdotL) = D * V * F
    // V = G / (4 * NdotV * NdotL)
    //   = 1 / ((NdotV * (2 - a) + a) * (NdotL * (2 - a) + a))

    // roughness = (roughness + 1) / 2, by Disney
    // a = roughness^2
    float r = (roughness + 1.0) * 0.5;
    float a = r * r;
    float denomV = NdotV * (2.0 - a) + a;
    float denomL = NdotL * (2.0 - a) + a;

    return 1.0 / (denomV * denomL);
}

vec3 FresnelSchlick(vec3 F0, float VdotH)
{
    return F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
}

vec3 FresnelSchlickRoughness(vec3 F0, float NdotV, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - NdotV, 5.0);
}  

#endif
