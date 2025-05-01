#include "ModelImporter.h"

#include "Core/Log.h"
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

        if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, 0), texture) == AI_SUCCESS)
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
    m_path(std::move(path)), m_pScene(nullptr)
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
    constexpr unsigned int ImportFlags =
        aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph |                        // General
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
    importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.0f);
    const aiScene *m_pScene = importer.ApplyPostProcessing(ImportFlags);
    if (!m_pScene)
    {
        SL_LOG_ERROR("Failed to process scene: {}", importer.GetErrorString());
        return false;
    }
    TraceSceneInfoBrief(m_pScene);
    TraceSceneInfoDetail(m_pScene);

    return true;
}

} // namespace sl
