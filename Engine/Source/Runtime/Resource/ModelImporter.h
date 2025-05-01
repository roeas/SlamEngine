#pragma once

#include <string>

struct aiScene;

namespace sl
{

class ModelImporter final
{
public:
    ModelImporter(std::string path);

    bool Import();

private:
    std::string m_path;

    aiScene *m_pScene;
};

} // namespace sl
