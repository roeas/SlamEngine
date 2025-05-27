#version 460 core

#include <Common.glsl>
#include <Camera.glsl>
#include <Light.glsl>
#include <Material.glsl>
#include <BRDF.glsl>

// Input
layout(location = 0) in vec3 v_worldPos;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tangent;
layout(location = 3) in vec3 v_bitangent;
layout(location = 4) in vec2 v_uv0;

// Output
layout(location = 0) out vec4 o_color;

// Texture
layout(binding = SL_SLOT_RADIANCE) uniform samplerCube s_radiance;
layout(binding = SL_SLOT_IRRADIANCE) uniform samplerCube s_irradiance;
layout(binding = SL_SLOT_IBL_LUT) uniform sampler2D s_IBL_LUT;

// Uniform
layout(location = SL_LOCATION_IBL_FACTOR) uniform float u_IBLFactor;

float GetDistanceAttenuation(float distance2, float range)
{
    // n = 4
    float attenuation = 1.0 / max(distance2, 0.01 * 0.01);
    float factor = distance2 / (range * range);
    float smoothFactor = clamp(1.0 - factor * factor, 0.0, 1.0);
    float smoothDistanceAtt = smoothFactor * smoothFactor;

    return attenuation * smoothDistanceAtt;
}

float GetAngleAttenuation(vec3 lightDir, vec3 lightForward, float angleScale, float angleOffset)
{
    // On CPU:
    // float angleScale = 1.0 / max((cosInner - cosOuter), 0.001);
    // float angleOffset = -cosOuter * angleScale;
    float cd = dot(lightDir, -lightForward);
    float attenuation = clamp(cd * angleScale + angleOffset, 0.0, 1.0);

    return attenuation * attenuation;
}

vec3 GetDirectBRDF(vec3 lightDir, vec3 viewDir, Material material)
{
    vec3 harfDir = normalize(lightDir + viewDir);
    float NdotV = clamp(dot(material.normal, viewDir), 0.0, 1.0);
    float NdotL = clamp(dot(material.normal, lightDir), 0.0, 1.0);
    float NdotH = clamp(dot(material.normal, harfDir), 0.0, 1.0);
    float VdotH = clamp(dot(viewDir, harfDir), 0.0, 1.0);

    float dis = DistributionGGX(material.roughness, NdotH);
    float vis = VisibilitySchlickSmith(material.roughness, NdotV, NdotL);
    vec3 fre = FresnelSchlick(material.F0, VdotH);
    vec3 kd = mix(vec3(1.0) - fre, vec3(0.0), material.metallic);

    return kd * material.albedo * vec3(SL_PI_INV) + vec3(dis) * vec3(vis) * fre;
}

vec3 EvalDirectLight(vec3 worldPos, vec3 cameraPos, Material material)
{
    vec3 directColor = vec3(0.0);

    for(int i = 0; i < ub_lightCount; ++i)
    {
        Light light = GetLight(ub_lights[i]);

        vec3 lightDirU = light.position - worldPos;
        vec3 lightDir = normalize(lightDirU);
        vec3 viewDir = normalize(cameraPos - worldPos);
        float NdotL = max(dot(material.normal, lightDir), 0.0);

        float distance2 = dot(lightDirU, lightDirU);
        float distanceAttenuation = GetDistanceAttenuation(distance2, light.range);
        float angleAttenuation = GetAngleAttenuation(lightDir, light.direction, light.scale, light.offset);

        switch(light.type)
        {
            case SL_TYPE_LIGHT_DIRECTIONAL:
            {
                // L_out = f * E * cosTheta

                lightDir = -light.direction;
                NdotL = max(dot(material.normal, lightDir), 0.0);
                vec3 BRDF = GetDirectBRDF(lightDir, viewDir, material);

                directColor += BRDF * light.color * light.intensity * NdotL;
                break;
            }
            case SL_TYPE_LIGHT_POINT:
            {
                // I = Phi / 4Pi
                // L_out = f * L_in * cosTheta

                vec3 BRDF = GetDirectBRDF(lightDir, viewDir, material);
                float I = light.intensity * 0.25 * SL_PI_INV;
                float L = I * distanceAttenuation;

                directColor += BRDF * light.color * L * NdotL;
                break;
            }
            case SL_TYPE_LIGHT_SPOT:
            {
                // I = Phi / Pi
                // L_out = f * L_in * cosTheta

                vec3 BRDF = GetDirectBRDF(lightDir, viewDir, material);
                float I = light.intensity * SL_PI_INV;
                float L = I * distanceAttenuation * angleAttenuation;

                directColor += BRDF * light.color * L * NdotL;
                break;
            }
            default:
            {
                directColor = vec3(0.0);
                break;
            }
        }
    }

    return directColor;
}

vec3 EvalEnvLight(vec3 viewDir, vec3 normal, Material material)
{
    vec3 fre = FresnelSchlickRoughness(material.F0, max(dot(normal, viewDir), 0.0), material.roughness);
    vec3 ks = fre;
    vec3 kd = 1.0 - ks;
    kd *= 1.0 - material.metallic; 

    vec3 reflectDir = reflect(-viewDir, normal);
    float mip = material.roughness * float(textureQueryLevels(s_radiance) - 1);
    vec3 radiance = textureLod(s_radiance, reflectDir, mip).xyz;
    vec3 irradiance = texture(s_irradiance, normal).xyz;
    vec2 lut = texture(s_IBL_LUT, vec2(max(dot(normal, viewDir), 0.0), material.roughness)).xy;
    vec3 specularBRDF = fre * lut.x + lut.y;

    vec3 envColor = kd * material.albedo * irradiance + ks * specularBRDF * radiance;
    return envColor * vec3(u_IBLFactor);
}

void main()
{
    vec3 cameraPos = GetCameraPos();
    Material material = GetMaterial(v_uv0, v_normal, v_tangent, v_bitangent);

    // Direct
    vec3 directColor = EvalDirectLight(v_worldPos, cameraPos, material);
    directColor *= vec3(material.occlusion);

    // Environment
    vec3 envColor = EvalEnvLight(normalize(cameraPos - v_worldPos), v_normal, material);

    vec3 finalColor = directColor + envColor + material.emissive;
    o_color = vec4(finalColor, 1.0);
}
