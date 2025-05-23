#ifndef SL_LIGHT_GLSL
#define SL_LIGHT_GLSL

#include "Shared.h"

layout(std140, binding = SL_BINDING_POINT_LIGHT) uniform UBLights
{
    SL_LightUniformBuffer ub_lights[SL_LIGHT_MAX_COUNT];
    int ub_lightCount;
};

struct Light
{
    int type;
    float intensity;
    float range;
    float scale;
    float offset;
    vec3 color;
    vec3 position;
    vec3 direction;
};

Light GetLight(SL_LightUniformBuffer ubLight)
{
    Light light;
    light.type = ubLight.type;
    light.intensity = ubLight.intensity;
    light.range = ubLight.range;
    light.scale = ubLight.scale;
    light.offset = ubLight.offset;
    light.color = vec3(ubLight.colorR, ubLight.colorG, ubLight.colorB);
    light.position = vec3(ubLight.positionX, ubLight.positionY, ubLight.positionZ);
    light.direction = vec3(ubLight.directionX, ubLight.directionY, ubLight.directionZ);

    return light;
}

#endif
