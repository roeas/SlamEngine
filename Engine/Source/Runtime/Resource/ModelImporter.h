#pragma once

#include "Utils/Hash.hpp"

#include <string>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

namespace sl
{

class ModelImporter final
{
public:
    ModelImporter(std::string path);

    void SetGlobalScale(float scale) { m_globalScale = scale; }
    bool Import();

private:
    void ProcessNode(const aiNode *pNode);
    void ProcessMesh(const aiMesh *pMesh);
    StringHashType ProcessMaterial(const aiMaterial *pMaterial);
    StringHashType ProcessTexture(const char *pTexture, uint32_t mapping);

    template<typename T>
    T CreatePropertyGroup(const aiMaterial *pMaterial, int type);

    std::string m_path;
    const aiScene *m_pScene;
    float m_globalScale;
};

} // namespace sl
