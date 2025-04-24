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

    void OnImport() override;
    void OnBuild() override;
    void OnLoad() override;
    void OnUpload() override;
    void OnReady() override;
    void OnDestroy() override;
    void DestroyCPUData() override;

    VertexArray *GetVertexArray() const { return m_pVertexArray.get(); }

    std::vector<float> m_verticesRawData;
    std::vector<uint32_t> m_indicesRawData;
    VertexLayout m_layout;
    uint32_t m_vertexCount;
    uint32_t m_indexCount;

    std::unique_ptr<VertexArray> m_pVertexArray;
};

} // namespace sl
