#pragma once

#include "Resource/Resource.h"
#include "Renderer/Layout.h"

#include <memory>

namespace sl
{

class VertexArray;

class MeshResource : public Resource
{
public:
    MeshResource(std::vector<float> vertices, std::vector<uint32_t> indices, VertexLayout layout);
    MeshResource(const MeshResource &) = delete;
    MeshResource &operator=(const MeshResource &) = delete;
    MeshResource(MeshResource &&) = delete;
    MeshResource &operator=(MeshResource &&) = delete;
    ~MeshResource() override;

    void SetName(std::string name) { m_name = std::move(name); }
    const std::string &GetName() const { return m_name; }
    uint32_t GetVerticesCount() const { return m_verticesCount; }
    uint32_t GetIndicesCount() const { return m_indicesCount; }

    VertexArray *GetVertexArray() const { return m_pVertexArray.get(); }

private:
    void OnImport() override;
    void OnBuild() override;
    void OnLoad() override;
    void OnUpload() override;
    void OnReady() override;
    void OnDestroy() override;
    void DestroyCPUData() override;

    std::string m_name;
    std::vector<float> m_vertices;
    std::vector<uint32_t> m_indices;
    VertexLayout m_layout;
    uint32_t m_verticesCount;
    uint32_t m_indicesCount;

    std::unique_ptr<VertexArray> m_pVertexArray;
};

} // namespace sl
