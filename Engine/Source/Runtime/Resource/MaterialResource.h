#pragma once

#include "Resource/Resource.h"
#include "Shader/Shared.h"
#include "Utils/Hash.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sl
{

/*
 * TODO: Material is a very complex concept.
 * We can provide some built-in shaders and store the corresponding materials as files,
 * and a more flexible way is to reflect any type of parameters from the user custom shader.
 * Anyway, lets support PBR material by hard coding at first.
 */

struct AlbedoPropertyGroup
{
    StringHashType m_textureID = 0;
    glm::vec3 m_factor{ 0.5f };

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
    float m_factor = 0.9f;

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
    float m_factor = 0.1f;

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

    void SetName(std::string name) { m_name = std::move(name); }
    const std::string &GetName() const { return m_name; }

    void SetAlbedoPropertyGroup(const AlbedoPropertyGroup &group) { m_albedoPropertyGroup = group; }
    void SetNormalPropertyGroup(const NormalPropertyGroup &group) { m_normalPropertyGroup = group; }
    void SetOcclusionPropertyGroup(const OcclusionPropertyGroup &group) { m_occlusionPropertyGroup = group; }
    void SetRoughnessPropertyGroup(const RoughnessPropertyGroup &group) { m_roughnessPropertyGroup = group; }
    void SetMetallicPropertyGroup(const MetallicPropertyGroup &group) { m_metallicPropertyGroup = group; }
    void SetEmissivePropertyGroup(const EmissivePropertyGroup &group) { m_emissivePropertyGroup = group; }

    AlbedoPropertyGroup &GetAlbedoPropertyGroup() { return m_albedoPropertyGroup; }
    NormalPropertyGroup &GetNormalPropertyGroup() { return m_normalPropertyGroup; }
    OcclusionPropertyGroup &GetOcclusionPropertyGroup() { return m_occlusionPropertyGroup; }
    RoughnessPropertyGroup &GetRoughnessPropertyGroup() { return m_roughnessPropertyGroup; }
    MetallicPropertyGroup &GetMetallicPropertyGroup() { return m_metallicPropertyGroup; }
    EmissivePropertyGroup &GetEmissivePropertyGroup() { return m_emissivePropertyGroup; }

    const AlbedoPropertyGroup &GetAlbedoPropertyGroup() const { return m_albedoPropertyGroup; }
    const NormalPropertyGroup &GetNormalPropertyGroup() const { return m_normalPropertyGroup; }
    const OcclusionPropertyGroup &GetOcclusionPropertyGroup() const { return m_occlusionPropertyGroup; }
    const RoughnessPropertyGroup &GetRoughnessPropertyGroup() const { return m_roughnessPropertyGroup; }
    const MetallicPropertyGroup &GetMetallicPropertyGroup() const { return m_metallicPropertyGroup; }
    const EmissivePropertyGroup &GetEmissivePropertyGroup() const { return m_emissivePropertyGroup; }

    void SetTwoSide(bool twoside) { m_twoSide = twoside; }
    bool &GetTwoSide() { return m_twoSide; }
    const bool &GetTwoSide() const { return m_twoSide; }

private:
    void OnImport() override;
    void OnBuild() override;
    void OnLoad() override;
    void OnUpload() override;
    void OnReady() override;
    void OnDestroy() override;
    void DestroyCPUData() override;

    std::string m_name;

    AlbedoPropertyGroup m_albedoPropertyGroup;
    NormalPropertyGroup m_normalPropertyGroup;
    OcclusionPropertyGroup m_occlusionPropertyGroup;
    RoughnessPropertyGroup m_roughnessPropertyGroup;
    MetallicPropertyGroup m_metallicPropertyGroup;
    EmissivePropertyGroup m_emissivePropertyGroup;

    bool m_twoSide = false;
};

} // namespace sl
