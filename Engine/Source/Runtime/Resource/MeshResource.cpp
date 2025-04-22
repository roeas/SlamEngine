#include "MeshResource.h"

#include "Core/Log.h"

namespace sl
{

MeshResource::MeshResource(std::vector<float> vertices, std::vector<uint32_t> indices, VertexLayout layout) :
    m_verticesRawData(std::move(vertices)), m_indicesRawData(std::move(indices)), m_layout(std::move(layout)),
    m_vertexCount((uint32_t)m_verticesRawData.size()), m_indexCount((uint32_t)m_indicesRawData.size())
{

}

MeshResource::~MeshResource()
{
    OnDestroy();
}

void MeshResource::OnImport()
{
    m_state = ResourceState::Building;
}

void MeshResource::OnBuild()
{
    m_state = ResourceState::Uploading;
}

void MeshResource::OnLoad()
{
    m_state = ResourceState::Uploading;
}

void MeshResource::OnUpload()
{
    sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(m_verticesRawData.data(), m_verticesRawData.size() * sizeof(float));
    sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(m_indicesRawData.data(), m_indicesRawData.size() * sizeof(uint32_t));
    m_pVertexArray.reset(sl::VertexArray::Create(pVertexBuffer, pIndexBuffer, m_layout));

    m_state = ResourceState::Ready;
}

void MeshResource::OnReady()
{
    if (!m_optimized) [[unlikely]]
    {
        if (!m_destroyDelayFrame--)
        {
            DestroyCPUData();
        }
    }
}

void MeshResource::OnDestroy()
{
    DestroyCPUData();
    m_pVertexArray.reset();
    m_state = ResourceState::Destroyed;
}

void MeshResource::DestroyCPUData()
{
    m_verticesRawData.clear();
    std::vector<float>().swap(m_verticesRawData);
    m_indicesRawData.clear();
    std::vector<uint32_t>().swap(m_indicesRawData);
}

} // namespace sl
