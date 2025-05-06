#include "ModelImporter.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Renderer/Layout.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"
#include "Utils/ProfilerCPU.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <magic_enum/magic_enum.hpp>

namespace sl
{

namespace
{

void LogSceneInfoBrief(const aiScene *pScene)
{
    SL_LOG_TRACE("\tScene name: {}", pScene->mName.C_Str());
    SL_LOG_TRACE("\tMesh count: {}", pScene->mNumMeshes);
    SL_LOG_TRACE("\tMaterial count: {}", pScene->mNumMaterials);
    SL_LOG_TRACE("\tEmbedded Texture count: {}", pScene->mNumTextures);
    SL_LOG_TRACE("\tSkeleton count: {}", pScene->mNumSkeletons);
    SL_LOG_TRACE("\tAnimation count: {}", pScene->mNumAnimations);
    SL_LOG_TRACE("\tLight count: {}", pScene->mNumLights);
    SL_LOG_TRACE("\tCamera count: {}", pScene->mNumCameras);
}

void LogSceneInfoDetail(const aiScene *pScene)
{
    // Vertices
    for (size_t i = 0; i < pScene->mNumMeshes; ++i)
    {
        aiMesh *pMesh = pScene->mMeshes[i];
        SL_LOG_TRACE("\tMesh: {}", pMesh->mName.C_Str());
        SL_LOG_TRACE("\t\tVertex count: {}", pMesh->mNumVertices);
        SL_LOG_TRACE("\t\tIndex count: {}", pMesh->mNumFaces * 3);
        if (auto count = pMesh->GetNumUVChannels(); count > 1)
        {
            SL_LOG_WARN("UV channels count: {}", count);
        }
    }

    // Textures
    auto LogTextures = [](aiMaterial *pMaterial, aiTextureType type, const char *pName)
    {
        uint32_t count = pMaterial->GetTextureCount(type);
        for (uint32_t index = 0; index < count; ++index)
        {
            if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(type, index), texture) == AI_SUCCESS)
            {
                if (index)
                {
                    SL_LOG_WARN("{} {}: {}", pName, index, texture.C_Str());
                }
                else
                {
                    SL_LOG_TRACE("\t\t{} {}: {}", pName, index, texture.C_Str());
                }
            }
        }
    };
    for (size_t i = 0; i < pScene->mNumMaterials; ++i)
    {
        aiMaterial *pMaterial = pScene->mMaterials[i];
        SL_LOG_TRACE("\tMaterial: {}", pMaterial->GetName().C_Str());

        // Assimp's textuere type matching is not trustworthy
        LogTextures(pMaterial, aiTextureType_NONE, "None");
        LogTextures(pMaterial, aiTextureType_DIFFUSE, "Diffuse");
        LogTextures(pMaterial, aiTextureType_SPECULAR, "Specular");
        LogTextures(pMaterial, aiTextureType_AMBIENT, "Ambient");
        LogTextures(pMaterial, aiTextureType_EMISSIVE, "Emissive");
        LogTextures(pMaterial, aiTextureType_HEIGHT, "Height");
        LogTextures(pMaterial, aiTextureType_NORMALS, "Normal");
        LogTextures(pMaterial, aiTextureType_SHININESS, "Shininess");
        LogTextures(pMaterial, aiTextureType_OPACITY, "Opacity");
        LogTextures(pMaterial, aiTextureType_DISPLACEMENT, "Displacement");
        LogTextures(pMaterial, aiTextureType_LIGHTMAP, "Lightmap");
        LogTextures(pMaterial, aiTextureType_REFLECTION, "Reflection");
        LogTextures(pMaterial, aiTextureType_BASE_COLOR, "Base Color");
        LogTextures(pMaterial, aiTextureType_NORMAL_CAMERA, "Noamel Camera");
        LogTextures(pMaterial, aiTextureType_EMISSION_COLOR, "Emission Color");
        LogTextures(pMaterial, aiTextureType_METALNESS, "Metalness");
        LogTextures(pMaterial, aiTextureType_DIFFUSE_ROUGHNESS, "Diffuse Roughness");
        LogTextures(pMaterial, aiTextureType_AMBIENT_OCCLUSION, "Ambient Occlusion");
        LogTextures(pMaterial, aiTextureType_UNKNOWN, "Unknown");
        LogTextures(pMaterial, aiTextureType_SHEEN, "Sheen");
        LogTextures(pMaterial, aiTextureType_CLEARCOAT, "Clearcoat");
        LogTextures(pMaterial, aiTextureType_TRANSMISSION, "Transmission");
        LogTextures(pMaterial, aiTextureType_MAYA_BASE, "Maya Base");
        LogTextures(pMaterial, aiTextureType_MAYA_SPECULAR, "Maya Specular");
        LogTextures(pMaterial, aiTextureType_MAYA_SPECULAR_COLOR, "Maya Specular Color");
        LogTextures(pMaterial, aiTextureType_MAYA_SPECULAR_ROUGHNESS, "Maya Specular Roughness");
    }

    // Embedded textures
    if (pScene->mNumTextures)
    {
        SL_LOG_WARN("No support for embedded texture");
        for (size_t i = 0; i < pScene->mNumTextures; ++i)
        {
            SL_LOG_WARN("Embedded Texture: {}", pScene->mTextures[i]->mFilename.C_Str());
        }
    }
}

uint32_t TextureMappingU[magic_enum::enum_count<aiTextureMapMode>()]
{
    SL_SAMPLER_U_REPEAT,
    SL_SAMPLER_U_CLAMP,
    SL_SAMPLER_U_MIRROR,
    SL_SAMPLER_U_BORDER,
};

uint32_t TextureMappingV[magic_enum::enum_count<aiTextureMapMode>()]
{
    SL_SAMPLER_V_REPEAT,
    SL_SAMPLER_V_CLAMP,
    SL_SAMPLER_V_MIRROR,
    SL_SAMPLER_V_BORDER,
};

} //namespace

ModelImporter::ModelImporter(std::string path) :
    m_path(std::move(path)), m_pScene(nullptr), m_globalScale(1.0f)
{

}

bool ModelImporter::Import()
{
    SL_PROFILE;
    SL_LOG_TRACE("Importing model \"{}\"", m_path.data());

    Assimp::Importer importer;
    const aiScene *pScene = importer.ReadFile(m_path.data(), 0);
    if (!pScene)
    {
        SL_LOG_ERROR("Failed to import model: {}", importer.GetErrorString());
        return false;
    }
    LogSceneInfoBrief(pScene);

    // TODO: Support animation, UV transforming and embedded texture
    // TODO: #aiProcess_OptimizeGraph and #aiProcess_PreTransformVertices are incompatible
    constexpr unsigned int ImportFlags =
        aiProcess_OptimizeMeshes | /* aiProcess_OptimizeGraph | */                  // General
        aiProcess_FindInvalidData | aiProcess_RemoveComponent |
        aiProcess_SplitLargeMeshes | aiProcess_FindInstances |                      // Mesh
        aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_FindDegenerates | // Face
        aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality |          // Vertex
        aiProcess_PreTransformVertices | aiProcess_GlobalScale |
        aiProcess_DropNormals | aiProcess_FixInfacingNormals |                      // Normal
        aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace |
        aiProcess_GenUVCoords | aiProcess_TransformUVCoords |                       // UV
        aiProcess_RemoveRedundantMaterials |                                        // Material
        aiProcess_GenBoundingBoxes |                                                // Bounding Box
        aiProcess_ValidateDataStructure;

    SL_LOG_TRACE("Processing scene");
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
        aiComponent::aiComponent_COLORS | aiComponent::aiComponent_LIGHTS | aiComponent::aiComponent_CAMERAS);
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
        aiPrimitiveType::aiPrimitiveType_POINT | aiPrimitiveType::aiPrimitiveType_LINE);
    importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, m_globalScale);
    m_pScene = importer.ApplyPostProcessing(ImportFlags);
    if (!m_pScene)
    {
        SL_LOG_ERROR("Failed to process scene: {}", importer.GetErrorString());
        return false;
    }
    LogSceneInfoBrief(m_pScene);
    LogSceneInfoDetail(m_pScene);

    ProcessNode(m_pScene->mRootNode);

    return true;
}

void ModelImporter::ProcessNode(const aiNode *pNode)
{
    for (size_t i = 0; i < pNode->mNumMeshes; ++i)
    {
        ProcessMesh(m_pScene->mMeshes[pNode->mMeshes[i]]);
    }

    for (size_t i = 0; i < pNode->mNumChildren; ++i)
    {
        ProcessNode(pNode->mChildren[i]);
    }
}

void ModelImporter::ProcessMesh(const aiMesh *pMesh)
{
    SL_ASSERT(pMesh->mPrimitiveTypes & aiPrimitiveType::aiPrimitiveType_TRIANGLE);
    SL_ASSERT(pMesh->mVertices && pMesh->mNormals && pMesh->mTangents && pMesh->mBitangents && pMesh->mTextureCoords[0]);

    // 1. Vertex
    std::vector<sl::VertexLayoutElement> elements
    {
        { "Position", 3, sl::AttribType::Float, false },
        { "Normal", 3, sl::AttribType::Float, false },
        { "Tangent", 3, sl::AttribType::Float, false },
        { "UV0", 2, sl::AttribType::Float, false },
    };
    sl::VertexLayout layout{ std::move(elements) };
    
    uint32_t vertexCount = pMesh->mNumVertices;
    uint32_t layoutfloatCount = layout.GetStride() / sizeof(float);
    std::vector<float> vertices;
    vertices.resize(vertexCount * layoutfloatCount);
    for (size_t i = 0; i < vertexCount; ++i)
    {
        memcpy(&vertices[i * layoutfloatCount + 0], &pMesh->mVertices[i], 3 * sizeof(float));
        memcpy(&vertices[i * layoutfloatCount + 3], &pMesh->mNormals[i], 3 * sizeof(float));
        memcpy(&vertices[i * layoutfloatCount + 6], &pMesh->mTangents[i], 3 * sizeof(float));
        memcpy(&vertices[i * layoutfloatCount + 9], &pMesh->mTextureCoords[0][i], 2 * sizeof(float));
    }
    static_assert(std::same_as<decltype((*pMesh->mVertices).x), float>);
    static_assert(std::same_as<decltype((*pMesh->mNormals).x), float>);
    static_assert(std::same_as<decltype((*pMesh->mTangents).x), float>);
    static_assert(std::same_as<decltype((**pMesh->mTextureCoords).x), float>);

    // 2. Index
    uint32_t faceCount = pMesh->mNumFaces;
    std::vector<uint32_t> indices;
    indices.resize(faceCount * 3);
    for (size_t i = 0; i < faceCount; ++i)
    {
        SL_ASSERT(pMesh->mFaces[i].mNumIndices == 3);
        memcpy(&indices[i * 3], pMesh->mFaces[i].mIndices, 3 * sizeof(uint32_t));
    }
    static_assert(std::same_as<decltype((*pMesh->mFaces).mIndices), uint32_t *>);

    // 3. Mesh resource
    const char *pMeshName = pMesh->mName.C_Str();
    StringHashType meshID = StringHash(m_path + pMeshName);
    auto pMeshResource = std::make_unique<sl::MeshResource>(std::move(vertices), std::move(indices), std::move(layout));
    pMeshResource->SetName(pMeshName);
    sl::ResourceManager::AddMeshResource(meshID, std::move(pMeshResource));

    // 4. Material resource
    StringHashType materialID = ProcessMaterial(m_pScene->mMaterials[pMesh->mMaterialIndex]);

    // 5. Entity and component
    auto &rendering = sl::World::CreateEntity(pMeshName).AddComponent<sl::RenderingComponent>();
    rendering.m_meshResourceID = meshID;
    rendering.m_shaderResourceID = sl::StringHash("Base Shader");
    rendering.m_entityIDShaderResourceID = sl::StringHash("EntityID Shader");
    rendering.m_materialResourceID = materialID;
}

StringHashType ModelImporter::ProcessMaterial(const aiMaterial *pMaterial)
{
    /*
     * Due to assimp's poor support for PBR texture, we have to get these textures with a weird mapping.
     * These #aiTextureType configurations apply to gltf, but other formats are untested.
     */
    auto pMaterialResource = std::make_unique<MaterialResource>();
    pMaterialResource->SetName(pMaterial->GetName().C_Str());

    pMaterialResource->SetAlbedoPropertyGroup(CreatePropertyGroup<AlbedoPropertyGroup>(pMaterial, aiTextureType_BASE_COLOR));
    pMaterialResource->SetNormalPropertyGroup(CreatePropertyGroup<NormalPropertyGroup>(pMaterial, aiTextureType_NORMALS));
    pMaterialResource->SetOcclusionPropertyGroup(CreatePropertyGroup<OcclusionPropertyGroup>(pMaterial, aiTextureType_LIGHTMAP));
    pMaterialResource->SetRoughnessPropertyGroup(CreatePropertyGroup<RoughnessPropertyGroup>(pMaterial, aiTextureType_DIFFUSE_ROUGHNESS));
    pMaterialResource->SetMetallicPropertyGroup(CreatePropertyGroup<MetallicPropertyGroup>(pMaterial, aiTextureType_METALNESS));
    pMaterialResource->SetEmissivePropertyGroup(CreatePropertyGroup<EmissivePropertyGroup>(pMaterial, aiTextureType_EMISSIVE));

    if (int twoSide; pMaterial->Get(AI_MATKEY_TWOSIDED, twoSide))
    {
        pMaterialResource->SetTwoSide((bool)twoSide);
    }

    StringHashType materialID = StringHash(m_path + pMaterial->GetName().C_Str());
    sl::ResourceManager::AddMaterialResource(materialID, std::move(pMaterialResource));

    return materialID;
}

StringHashType ModelImporter::ProcessTexture(const char *pTexture, uint32_t mapping)
{
    std::string filePath = Path::Join(Path::Parent(m_path), pTexture);
    StringHashType textureID = StringHash(filePath);
    if (!sl::ResourceManager::GetTextureResource(textureID))
    {
        auto pTextureResource = std::make_unique<sl::TextureResource>(std::move(filePath), true, mapping | SL_SAMPLER_LINEAR);
        pTextureResource->SetName(Path::NameWithoutExtension(pTexture));
        sl::ResourceManager::AddTextureResource(textureID, std::move(pTextureResource));
    }

    return textureID;
}

template<typename T>
T ModelImporter::CreatePropertyGroup(const aiMaterial *pMaterial, int type)
{
    T propertyGroup;
    if (aiString textureStr; pMaterial->Get(AI_MATKEY_TEXTURE(type, 0), textureStr) == AI_SUCCESS)
    {
        int mappingU = 0; // SL_SAMPLER_U_REPEAT
        int mappingV = 0; // SL_SAMPLER_V_REPEAT
        if (int mapping; pMaterial->Get(AI_MATKEY_MAPPINGMODE_U(type, 0), mapping) == AI_SUCCESS)
        {
            mappingU = mapping;
        }
        if (int mapping; pMaterial->Get(AI_MATKEY_MAPPINGMODE_V(type, 0), mapping) == AI_SUCCESS)
        {
            mappingV = mapping;
        }

        propertyGroup.m_textureID = ProcessTexture(textureStr.C_Str(), TextureMappingU[mappingU] | TextureMappingV[mappingV]);
        propertyGroup.m_useTexture = true;
    }
    if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(type, 0), transform) == AI_SUCCESS)
    {
        propertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
        propertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
        propertyGroup.m_rotation = transform.mRotation;
    }

    return propertyGroup;
}

} // namespace sl
