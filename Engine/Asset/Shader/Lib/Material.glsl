#ifndef SL_MATERIAL_GLSL
#define SL_MATERIAL_GLSL

#include "Shared.h"

// Sampler
layout(binding = SL_SLOT_ALBEDO) uniform sampler2D s_albedo;
layout(binding = SL_SLOT_NORMAL) uniform sampler2D s_normal;
layout(binding = SL_SLOT_ORM) uniform sampler2D s_ORM;
layout(binding = SL_SLOT_EMISSIVE) uniform sampler2D s_emissive;

// Uniform
layout(location = SL_LOCATION_ALBEDO_FACTOR) uniform vec3 u_albedoFactor;
layout(location = SL_LOCATION_NORMAL_FACTOR) uniform vec3 u_normalFactor;
layout(location = SL_LOCATION_OCCLUSION_FACTOR) uniform float u_occlusionFactor;
layout(location = SL_LOCATION_ROUGHNESS_FACTOR) uniform float u_roughnessFactor;
layout(location = SL_LOCATION_METALLIC_FACTOR) uniform float u_metallicFactor;
layout(location = SL_LOCATION_EMISSIVE_FACTOR) uniform vec3 u_emissiveFactor;

layout(location = SL_LOCATION_ALBEDO_USE_TEXTURE) uniform bool u_useAlbedoTexture;
layout(location = SL_LOCATION_NORMAL_USE_TEXTURE) uniform bool u_useNormalTexture;
layout(location = SL_LOCATION_OCCLUSION_USE_TEXTURE) uniform bool u_useOcclusionTexture;
layout(location = SL_LOCATION_ROUGHNESS_USE_TEXTURE) uniform bool u_useRoughnessTexture;
layout(location = SL_LOCATION_METALLIC_USE_TEXTURE) uniform bool u_useMetallicTexture;
layout(location = SL_LOCATION_EMISSIVE_USE_TEXTURE) uniform bool u_useEmissiveTexture;

vec3 SampleAlbedoTexture(vec2 uv)
{
    // TODO: Compile albedo textures to linear space before rendering
    return pow(texture(s_albedo, uv).xyz, vec3(2.2));
}
vec3 SampleNormalTexture(vec2 uv)
{
    return normalize(texture(s_normal, uv).xyz * 2.0 - 1.0);
}
vec3 SampleORMTexture(vec2 uv)
{
    return texture(s_ORM, uv).xyz;
}
vec3 SampleEmissiveTexture(vec2 uv)
{
    // TODO: Compile emissive textures to linear space before rendering
    return pow(texture(s_emissive, uv).xyz, vec3(2.2));
}

struct Material
{
    vec3 albedo;
    vec3 normal;
    float occlusion;
    float roughness;
    float metallic;
    vec3 emissive;
};

Material GetMaterial(vec2 uv, vec3 normal, vec3 tangent, vec3 bitangent)
{
    Material material;
    material.albedo = u_albedoFactor;
    material.normal = normalize(normal * u_normalFactor);
    material.occlusion = u_occlusionFactor;
    material.roughness = u_roughnessFactor;
    material.metallic = u_metallicFactor;
    material.emissive = u_emissiveFactor;
    
    if (u_useAlbedoTexture)
    {
    	material.albedo *= SampleAlbedoTexture(uv);
    }
    if (u_useNormalTexture)
    {
        mat3 TBN = mat3(tangent, bitangent, normal);
        vec3 normalMap = SampleNormalTexture(uv);
        material.normal = normalize(TBN * normalMap);
        material.normal = normalize(material.normal * u_normalFactor);
    }
    if(!gl_FrontFacing)
    {
        material.normal = -material.normal;
    }
    vec3 orm = SampleORMTexture(uv);
    if (u_useOcclusionTexture)
    {
    	material.occlusion *= orm.x;
    }
    if (u_useRoughnessTexture)
    {
    	material.roughness *= orm.y;
    }
    if (u_useMetallicTexture)
    {
    	material.metallic *= orm.z;
    }
    if (u_useEmissiveTexture)
    {
    	material.emissive *= SampleEmissiveTexture(uv);
    }

    return material;
}

#endif
