#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{
	// Color
	m_params.vec4Param[0] = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

	
}

Material::~Material()
{
}

void Material::Save(const wstring& path)
{
}

void Material::Load(const wstring& path)
{
}

void Material::PushGraphicsData()
{
	/* ----- ConstantBuffer(CBV) 정보 기입 ----- */ 
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushGraphicsData(&m_params, sizeof(m_params));

	/* ----- TextureBuffer(SRV) 정보 기입 ----- */
	for (size_t i = 0; i < m_textures.size(); ++i) {
		if (m_textures[i] == nullptr) {
			continue;
		}

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->GetGraphicsDescHeap()->SetSRV(m_textures[i]->GetSRVCpuHandle(), reg);
	}

	/* ----- Shader 정보 기입 ----- */
	m_shader->Update();
}

void Material::PushComputeData()
{
	// CBV 업로드
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushComputeData(&m_params, sizeof(m_params));

	// SRV 업로드
	for (size_t i = 0; i < m_textures.size(); i++) {
		if (m_textures[i] == nullptr) {
			continue;
		}

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->GetComputeDescHeap()->SetSRV(m_textures[i]->GetSRVCpuHandle(), reg);
	}

	// 파이프라인 세팅-
	m_shader->Update();
}

void Material::Dispatch(uint32 x, uint32 y, uint32 z)
{
	PushComputeData();

	// SetDescriptorHeaps + SetComputeRootDescriptorTable
	GEngine->GetComputeDescHeap()->CommitTable();

	COMPUTE_CMD_LIST->Dispatch(x, y, z);

	GEngine->GetComputeCmdQueue()->FlushResourceCommnadQueue();
}

Ref<Material> Material::Clone()
{
	Ref<Material> material = make_shared<Material>();
	material->SetShader(m_shader);
	material->m_params = m_params;
	material->m_textures = m_textures;
	material->name = m_name;

	return material;
}