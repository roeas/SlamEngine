#pragma once

#include "Resource/Resource.h"
#include "Shader/Shared.h"
#include "Utils/Hash.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sl
{

struct AlbedoPropertyGroup
{
    StringHashType m_textureID = 0;
    glm::vec3 m_factor{ 1.0f };

    glm::vec2 m_offset{ 0.0f };
    glm::vec2 m_scale{ 1.0f };
    float m_rotation = 0.0f;

    int m_textureSlot = SL_SLOT_ALBEDO;
    int m_useTextureLocation = SL_LOCATION_ALBEDO_USE_TEXTURE;
    int m_factorLocation = SL_LOCATION_ALBEDO_FACTOR;
    int m_tillingLocation = SL_LOCATION_ALBEDO_TILLING;

    bool m_useTexture = false;
};

struct NormalPropertyGroup
{
    StringHashType m_textureID = 0;
    glm::vec3 m_factor{ 1.0f };

    glm::vec2 m_offset{ 0.0f };
    glm::vec2 m_scale{ 1.0f };
    float m_rotation = 0.0f;

    int m_textureSlot = SL_SLOT_NORMAL;
    int m_useTextureLocation = SL_LOCATION_NORMAL_USE_TEXTURE;
    int m_factorLocation = SL_LOCATION_NORMAL_FACTOR;
    int m_tillingLocation = SL_LOCATION_NORMAL_TILLING;

    bool m_useTexture = false;
};

struct OcclusionPropertyGroup
{
    StringHashType m_textureID = 0;
    float m_factor = 1.0f;

    glm::vec2 m_offset{ 0.0f };
    glm::vec2 m_scale{ 1.0f };
    float m_rotation = 0.0f;

    int m_textureSlot = SL_SLOT_ORM;
    int m_useTextureLocation = SL_LOCATION_OCCLUSION_USE_TEXTURE;
    int m_factorLocation = SL_LOCATION_OCCLUSION_FACTOR;
    int m_tillingLocation = SL_LOCATION_OCCLUSION_TILLING;

    bool m_useTexture = false;
};

struct RoughnessPropertyGroup
{
    StringHashType m_textureID = 0;
    float m_factor = 1.0f;

    glm::vec2 m_offset{ 0.0f };
    glm::vec2 m_scale{ 1.0f };
    float m_rotation = 0.0f;

    int m_textureSlot = SL_SLOT_ORM;
    int m_useTextureLocation = SL_LOCATION_ROUGHNESS_USE_TEXTURE;
    int m_factorLocation = SL_LOCATION_ROUGHNESS_FACTOR;
    int m_tillingLocation = SL_LOCATION_ROUGHNESS_TILLING;

    bool m_useTexture = false;
};

struct MetallicPropertyGroup
{
    StringHashType m_textureID = 0;
    float m_factor = 1.0f;

    glm::vec2 m_offset{ 0.0f };
    glm::vec2 m_scale{ 1.0f };
    float m_rotation = 0.0f;

    int m_textureSlot = SL_SLOT_ORM;
    int m_useTextureLocation = SL_LOCATION_METALLIC_USE_TEXTURE;
    int m_factorLocation = SL_LOCATION_METALLIC_FACTOR;
    int m_tillingLocation = SL_LOCATION_METALLIC_TILLING;

    bool m_useTexture = false;
};

struct EmissivePropertyGroup
{
    StringHashType m_textureID = 0;
    glm::vec3 m_factor{ 0.0f };

    glm::vec2 m_offset{ 0.0f };
    glm::vec2 m_scale{ 1.0f };
    float m_rotation = 0.0f;

    int m_textureSlot = SL_SLOT_EMISSIVE;
    int m_useTextureLocation = SL_LOCATION_EMISSIVE_USE_TEXTURE;
    int m_factorLocation = SL_LOCATION_EMISSIVE_FACTOR;
    int m_tillingLocation = SL_LOCATION_EMISSIVE_TILLING;

    bool m_useTexture = false;
};

class MaterialResource : public Resource
{
public:
    MaterialResource() = default;
    MaterialResource(const MaterialResource &) = delete;
    MaterialResource &operator=(const MaterialResource &) = delete;
    MaterialResource(MaterialResource &&) = delete;
    MaterialResource &operator=(MaterialResource &&) = delete;
    ~MaterialResource() override;

    void SetAlbedoPropertyGroup(const AlbedoPropertyGroup &group) { m_albedoPropertyGroup = group; }
    void SetNormalPropertyGroup(const NormalPropertyGroup &group) { m_normalPropertyGroup = group; }
    void SetOcclusionPropertyGroup(const OcclusionPropertyGroup &group) { m_occlusionPropertyGroup = group; }
    void SetRoughnessPropertyGroup(const RoughnessPropertyGroup &group) { m_roughnessPropertyGroup = group; }
    void SetMetallicPropertyGroup(const MetallicPropertyGroup &group) { m_metallicPropertyGroup = group; }
    void SetEmissivePropertyGroup(const EmissivePropertyGroup &group) { m_emissivePropertyGroup = group; }

    const AlbedoPropertyGroup &GetAlbedoPropertyGroup(){ return m_albedoPropertyGroup; }
    const NormalPropertyGroup &GetNormalPropertyGroup(){ return m_normalPropertyGroup; }
    const OcclusionPropertyGroup &GetOcclusionPropertyGroup(){ return m_occlusionPropertyGroup; }
    const RoughnessPropertyGroup &GetRoughnessPropertyGroup(){ return m_roughnessPropertyGroup; }
    const MetallicPropertyGroup &GetMetallicPropertyGroup(){ return m_metallicPropertyGroup; }
    const EmissivePropertyGroup &GetEmissivePropertyGroup() { return m_emissivePropertyGroup; }

    void SetTwoSide(bool twoside) { m_twoSide = twoside; }

private:
    void OnImport() override;
    void OnBuild() override;
    void OnLoad() override;
    void OnUpload() override;
    void OnReady() override;
    void OnDestroy() override;
    void DestroyCPUData() override;

    // TODO: Material is a very complex concept,
    // a better design would be to reflect any type of parameters from shader.
    // Anyway, lets support PBRMaterial by hard coding at first.
    AlbedoPropertyGroup m_albedoPropertyGroup;
    NormalPropertyGroup m_normalPropertyGroup;
    OcclusionPropertyGroup m_occlusionPropertyGroup;
    RoughnessPropertyGroup m_roughnessPropertyGroup;
    MetallicPropertyGroup m_metallicPropertyGroup;
    EmissivePropertyGroup m_emissivePropertyGroup;

    bool m_twoSide = false;
};

} // namespace sl
