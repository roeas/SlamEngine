#include "ModelImporter.h"

#include "Core/Log.h"
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

void TraceSceneInfoBrief(const aiScene *pScene)
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

void TraceSceneInfoDetail(const aiScene *pScene)
{
    for (size_t i = 0; i < pScene->mNumMeshes; ++i)
    {
        aiMesh *pMesh = pScene->mMeshes[i];
        SL_LOG_TRACE("\tMesh: {}", pMesh->mName.C_Str());
        SL_LOG_TRACE("\t\tVertex count: {}", pMesh->mNumVertices);
        SL_LOG_TRACE("\t\tIndex count: {}", pMesh->mNumFaces * 3);
    }

    for (size_t i = 0; i < pScene->mNumMaterials; ++i)
    {
        aiMaterial *pMaterial = pScene->mMaterials[i];
        SL_LOG_TRACE("\tMaterial: {}", pMaterial->GetName().C_Str());

        if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture) == AI_SUCCESS)
        {
            SL_LOG_TRACE("\t\tAlbedo: {}", texture.C_Str());
        }
        if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), texture) == AI_SUCCESS)
        {
            SL_LOG_TRACE("\t\tNormal: {}", texture.C_Str());
        }
        if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_METALNESS, 0), texture) == AI_SUCCESS)
        {
            SL_LOG_TRACE("\t\tMetallic: {}", texture.C_Str());
        }
        if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE_ROUGHNESS, 0), texture) == AI_SUCCESS)
        {
            SL_LOG_TRACE("\t\tRoughness: {}", texture.C_Str());
        }
        if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP, 0), texture) == AI_SUCCESS)
        {
            SL_LOG_TRACE("\t\tOcclusion: {}", texture.C_Str());
        }
        if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), texture) == AI_SUCCESS)
        {
            SL_LOG_TRACE("\t\tEmission: {}", texture.C_Str());
        }
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
    TraceSceneInfoBrief(pScene);

    // TODO: Support animation, UV transforming and embedded texture
    // TODO: #aiProcess_OptimizeGraph and #aiProcess_PreTransformVertices are incompatible
    constexpr unsigned int ImportFlags =
        aiProcess_OptimizeMeshes | 0 |                                              // General
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
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent::aiComponent_COLORS | aiComponent::aiComponent_LIGHTS | aiComponent::aiComponent_CAMERAS);
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType::aiPrimitiveType_POINT | aiPrimitiveType::aiPrimitiveType_LINE);
    importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, m_globalScale);
    m_pScene = importer.ApplyPostProcessing(ImportFlags);
    if (!m_pScene)
    {
        SL_LOG_ERROR("Failed to process scene: {}", importer.GetErrorString());
        return false;
    }
    TraceSceneInfoBrief(m_pScene);
    TraceSceneInfoDetail(m_pScene);

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
    std::string materialResourceName = ProcessMaterial(m_pScene->mMaterials[pMesh->mMaterialIndex]);

    // 5. Entity and component
    auto &rendering = sl::World::CreateEntity(pMeshName).AddComponent<sl::RenderingComponent>();
    rendering.m_meshResourceID = meshNameHash;
    rendering.m_textureResourceID = 0;
    rendering.m_shaderResourceID = sl::StringHash("Base Shader");
    rendering.m_entityIDShaderResourceID = sl::StringHash("EntityID Shader");
}

std::string ModelImporter::ProcessMaterial(const aiMaterial *pMaterial)
{
    return "";
}

std::string ModelImporter::ProcessTexture(const char *textureStr)
{
    return "";
}

} // namespace sl
