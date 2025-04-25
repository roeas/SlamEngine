#include "MeshResource.h"

#include "Core/Log.h"
#include "Renderer/VertexArray.h"

namespace sl
{

MeshResource::MeshResource(std::vector<float> vertices, std::vector<uint32_t> indices, VertexLayout layout) :
    m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_layout(std::move(layout)),
    m_vertexCount((uint32_t)m_vertices.size()), m_indexCount((uint32_t)m_indices.size())
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
    std::unique_ptr<sl::VertexBuffer> pVertexBuffer{ sl::VertexBuffer::Create(m_vertices) };
    std::unique_ptr<sl::IndexBuffer> pIndexBuffer{ sl::IndexBuffer::Create(m_indices) };
    m_pVertexArray.reset(sl::VertexArray::Create(std::move(pVertexBuffer), std::move(pIndexBuffer), m_layout));

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
    m_vertices.clear();
    std::vector<float>().swap(m_vertices);
    m_indices.clear();
    std::vector<uint32_t>().swap(m_indices);

    m_optimized = true;
}

} // namespace sl
