#include "MeshResource.h"

#include "Core/Log.h"
#include "RenderCore/VertexArray.h"
#include "Utils/ProfilerCPU.h"

namespace sl
{

MeshResource::MeshResource(std::string_view path) :
	m_assetPath(path)
{
	SetState(ResourceState::Importing);
}

MeshResource::~MeshResource()
{
	OnDestroy();
}

void MeshResource::OnImport()
{
	SL_PROFILE;

	SetState(ResourceState::Building);
}

void MeshResource::OnBuild()
{
	SL_PROFILE;

	SetState(ResourceState::Uploading);
}

void MeshResource::OnLoad()
{
	SL_PROFILE;

	SetState(ResourceState::Uploading);
}

void MeshResource::OnUpload()
{
	SL_PROFILE;

	sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(m_verticesRowData.data(), m_verticesRowData.size() * sizeof(float));
	pVertexBuffer->SetLayout(m_layout);
	sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(m_indicesRowData.data(), m_indicesRowData.size() * sizeof(uint32_t));

	m_pVertexArray.reset(sl::VertexArray::Create());
	m_pVertexArray->SetVertexBuffer(pVertexBuffer);
	m_pVertexArray->SetIndexBuffer(pIndexBuffer);

	SetState(ResourceState::Ready);
}

void MeshResource::OnReady()
{
	static uint8_t frameCount = 0;
	if (frameCount <= 60 && frameCount++ == 60)
	{
		DestroyCPUData();
	}
}

void MeshResource::OnDestroy()
{
	SL_PROFILE;

	DestroyCPUData();
	m_pVertexArray.reset();

	SetState(ResourceState::Destroyed);
}

void MeshResource::DestroyCPUData()
{
	SL_PROFILE;

	m_verticesRowData.clear();
	std::vector<float>().swap(m_verticesRowData);
	m_indicesRowData.clear();
	std::vector<uint32_t>().swap(m_indicesRowData);
}

} // namespace sl
