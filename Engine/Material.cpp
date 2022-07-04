#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{
	// Color
	_params.vec4Param[0] = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

	
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
	/* ----- ConstantBuffer(CBV) ���� ���� ----- */ 
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushGraphicsData(&_params, sizeof(_params));

	/* ----- TextureBuffer(SRV) ���� ���� ----- */
	for (size_t i = 0; i < _textures.size(); ++i) {
		if (_textures[i] == nullptr) {
			continue;
		}

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->GetGraphicsDescHeap()->SetSRV(_textures[i]->GetSRVCpuHandle(), reg);
	}

	/* ----- Shader ���� ���� ----- */
	_shader->Update();
}

void Material::PushComputeData()
{
	// CBV ���ε�
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushComputeData(&_params, sizeof(_params));

	// SRV ���ε�
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->GetComputeDescHeap()->SetSRV(_textures[i]->GetSRVCpuHandle(), reg);
	}

	// ���������� ����-
	_shader->Update();
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
	material->SetShader(_shader);
	material->_params = _params;
	material->_textures = _textures;
	material->SetName(_name);

	return material;
}