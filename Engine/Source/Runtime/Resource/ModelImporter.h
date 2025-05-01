#pragma once

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
    std::string ProcessMaterial(const aiMaterial *pMaterial);
    std::string ProcessTexture(const char *textureStr);

    std::string m_path;
    const aiScene *m_pScene;
    float m_globalScale;
};

} // namespace sl
