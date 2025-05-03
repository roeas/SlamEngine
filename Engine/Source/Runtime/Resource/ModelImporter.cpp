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
    for (size_t i = 0; i < pScene->mNumMeshes; ++i)
    {
        aiMesh *pMesh = pScene->mMeshes[i];
        SL_LOG_TRACE("\tMesh: {}", pMesh->mName.C_Str());
        SL_LOG_TRACE("\t\tVertex count: {}", pMesh->mNumVertices);
        SL_LOG_TRACE("\t\tIndex count: {}", pMesh->mNumFaces * 3);
    }

    auto LogTextures = [](aiMaterial *pMaterial, aiTextureType type, const char *pName)
    {
        if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(type, 0), texture) == AI_SUCCESS)
        {
            SL_LOG_TRACE("\t\t{}: {}", pName, texture.C_Str());
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
    }

    if (pScene->mNumTextures)
    {
        SL_LOG_WARN("No support for embedded texture");
        for (size_t i = 0; i < pScene->mNumTextures; ++i)
        {
            SL_LOG_WARN("Embedded Texture: {}", pScene->mTextures[i]->mFilename.C_Str());
        }
    }
}

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
        int meshIndex = pNode->mMeshes[i];
        const aiMesh *pMesh = m_pScene->mMeshes[meshIndex];
        ProcessMesh(pMesh);
    }

    for (size_t i = 0; i < pNode->mNumChildren; ++i)
    {
        const aiNode *pChildNode = pNode->mChildren[i];
        ProcessNode(pChildNode);
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
    static_assert(std::same_as<std::remove_pointer_t<decltype((*pMesh->mFaces).mIndices)>, uint32_t>);

    // 3. Mesh resource
    const char *pMeshName = pMesh->mName.C_Str();
    StringHashType meshNameHash = StringHash(m_path + pMeshName);
    auto pMeshResource = std::make_unique<sl::MeshResource>(std::move(vertices), std::move(indices), std::move(layout));
    sl::ResourceManager::AddMeshResource(meshNameHash, std::move(pMeshResource));

    // 4. Material resource
    StringHashType materialResourceName = ProcessMaterial(m_pScene->mMaterials[pMesh->mMaterialIndex]);

    // 5. Entity and component
    auto &rendering = sl::World::CreateEntity(pMeshName).AddComponent<sl::RenderingComponent>();
    rendering.m_meshResourceID = meshNameHash;
    rendering.m_shaderResourceID = sl::StringHash("Base Shader");
    rendering.m_entityIDShaderResourceID = sl::StringHash("EntityID Shader");
    rendering.m_materialResourceID = materialResourceName;
}

StringHashType ModelImporter::ProcessMaterial(const aiMaterial *pMaterial)
{
    // Albedo
    AlbedoPropertyGroup albedoPropertyGroup;
    if (aiString textureStr; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), textureStr) == AI_SUCCESS)
    {
        albedoPropertyGroup.m_textureID = ProcessTexture(textureStr.C_Str());
        albedoPropertyGroup.m_useTexture = true;
    }
    if (aiColor3D factor; pMaterial->Get(AI_MATKEY_BASE_COLOR, factor) == AI_SUCCESS)
    {
        albedoPropertyGroup.m_factor = glm::vec3{ factor.r, factor.g , factor.b };
    }
    if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE, 0), transform) == AI_SUCCESS)
    {
        albedoPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
        albedoPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
        albedoPropertyGroup.m_rotation = transform.mRotation;
    }

    // Normal
    NormalPropertyGroup normalPropertyGroup;
    if (aiString textureStr; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), textureStr) == AI_SUCCESS)
    {
        normalPropertyGroup.m_textureID = ProcessTexture(textureStr.C_Str());
        normalPropertyGroup.m_useTexture = true;
    }
    if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_NORMALS, 0), transform) == AI_SUCCESS)
    {
        normalPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
        normalPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
        normalPropertyGroup.m_rotation = transform.mRotation;
    }

    // Occlusion
    OcclusionPropertyGroup occlusionPropertyGroup;
    if (aiString textureStr; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP, 0), textureStr) == AI_SUCCESS)
    {
        occlusionPropertyGroup.m_textureID = ProcessTexture(textureStr.C_Str());
        occlusionPropertyGroup.m_useTexture = true;
    }
    if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_LIGHTMAP, 0), transform) == AI_SUCCESS)
    {
        occlusionPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
        occlusionPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
        occlusionPropertyGroup.m_rotation = transform.mRotation;
    }

    // Roughness
    RoughnessPropertyGroup roughnessPropertyGroup;
    if (aiString textureStr; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE_ROUGHNESS, 0), textureStr) == AI_SUCCESS)
    {
        roughnessPropertyGroup.m_textureID = ProcessTexture(textureStr.C_Str());
        roughnessPropertyGroup.m_useTexture = true;
    }
    if (float factor; pMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, factor) == AI_SUCCESS)
    {
        roughnessPropertyGroup.m_factor = factor;
    }
    if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE_ROUGHNESS, 0), transform) == AI_SUCCESS)
    {
        roughnessPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
        roughnessPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
        roughnessPropertyGroup.m_rotation = transform.mRotation;
    }

    // Metallic
    MetallicPropertyGroup metallicPropertyGroup;
    if (aiString textureStr; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_METALNESS, 0), textureStr) == AI_SUCCESS)
    {
        metallicPropertyGroup.m_textureID = ProcessTexture(textureStr.C_Str());
        metallicPropertyGroup.m_useTexture = true;
    }
    if (float factor; pMaterial->Get(AI_MATKEY_METALLIC_FACTOR, factor) == AI_SUCCESS)
    {
        metallicPropertyGroup.m_factor = factor;
    }
    if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_METALNESS, 0), transform) == AI_SUCCESS)
    {
        metallicPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
        metallicPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
        metallicPropertyGroup.m_rotation = transform.mRotation;
    }

    // Emissive
    EmissivePropertyGroup emissivePropertyGroup;
    if (aiString textureStr; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), textureStr) == AI_SUCCESS)
    {
        emissivePropertyGroup.m_textureID = ProcessTexture(textureStr.C_Str());
        emissivePropertyGroup.m_useTexture = true;
        emissivePropertyGroup.m_factor = glm::vec3{ 1.0f };
    }
    if (aiColor3D factor; pMaterial->Get(AI_MATKEY_EMISSIVE_INTENSITY, factor) == AI_SUCCESS)
    {
        emissivePropertyGroup.m_factor = glm::vec3{ factor.r, factor.g , factor.b };
    }
    if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_EMISSIVE, 0), transform) == AI_SUCCESS)
    {
        emissivePropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
        emissivePropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
        emissivePropertyGroup.m_rotation = transform.mRotation;
    }

    // Twoside
    bool isTwoSide = false;
    if (int twoSide; pMaterial->Get(AI_MATKEY_TWOSIDED, twoSide))
    {
        isTwoSide = (bool)twoSide;
    }
    
    auto pMaterialResource = std::make_unique<MaterialResource>();
    pMaterialResource->SetAlbedoPropertyGroup(albedoPropertyGroup);
    pMaterialResource->SetNormalPropertyGroup(normalPropertyGroup);
    pMaterialResource->SetOcclusionPropertyGroup(occlusionPropertyGroup);
    pMaterialResource->SetRoughnessPropertyGroup(roughnessPropertyGroup);
    pMaterialResource->SetMetallicPropertyGroup(metallicPropertyGroup);
    pMaterialResource->SetEmissivePropertyGroup(emissivePropertyGroup);
    pMaterialResource->SetTwoSide(isTwoSide);

    // Material resource
    StringHashType hash = StringHash(m_path + pMaterial->GetName().C_Str());
    sl::ResourceManager::AddMaterialResource(hash, std::move(pMaterialResource));

    return hash;
}

StringHashType ModelImporter::ProcessTexture(const char *pTexture)
{
    std::string filePath = Path::Join(Path::Parent(m_path), pTexture);
    StringHashType hash = StringHash(filePath);
    if (!sl::ResourceManager::GetTextureResource(hash))
    {
        auto pTextureResource = std::make_unique<sl::TextureResource>(std::move(filePath), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
        sl::ResourceManager::AddTextureResource(hash, std::move(pTextureResource));
    }

    return hash;
}

} // namespace sl
