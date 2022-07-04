#include "pch.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "InstancingBuffer.h"
#include "Resources.h"
#include "Animator.h"

MeshRenderer::MeshRenderer() :
	Component(COMPONENT_TYPE::MESH_RENDERER)
{
	_shadow = GET_SINGLE(Resources)->Get<Material>(L"Shadow");
	_pick = GET_SINGLE(Resources)->Get<Material>(L"Pick");
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Render()
{
	for (uint32 i = 0; i < _materials.size(); ++i) {
		Ref<Material>& material = _materials[i];

		if (material == nullptr || material->GetShader() == nullptr) {
			continue;
		}

		GetTransform()->PushData();

		Ref<Animator> animator = GetGameObject()->GetAnimator();
		if (animator) {
			animator->PushData();
			material->SetInt(1, 1);
		}

		material->PushGraphicsData();
		_mesh->Render(1, i);
	}
}

void MeshRenderer::Render(Ref<InstancingBuffer>& buffer)
{
	for (uint32 i = 0; i < _materials.size(); ++i) {
		Ref<Material>& material = _materials[i];

		if (material == nullptr || material->GetShader() == nullptr) {
			continue;
		}

		buffer->PushData();

		Ref<Animator> animator = GetGameObject()->GetAnimator();
		if (animator) {
			animator->PushData();
			material->SetInt(1, 1);
		}

		material->PushGraphicsData();
		_mesh->Render(buffer, i);
	}
}

void MeshRenderer::RenderShadow()
{
	GetTransform()->PushData();
	_shadow->PushGraphicsData();
	_mesh->Render();
}

void MeshRenderer::RenderPick(uint32 index)
{
	PickInfo info;
	info.index = index;
	_pick->SetVec4(0, Vec4(info.r, info.g, info.b, info.a));

	GetTransform()->PushData();
	_pick->PushGraphicsData();
	_mesh->Render();
}

void MeshRenderer::Serializer(Json::Value& object)
{
	Json::Value& meshRendererInfo = object["Transform"];
	//{
	//	Json::Value scale;
	//	scale.append(_localScale.x);
	//	scale.append(_localScale.y);
	//	scale.append(_localScale.z);
	//	transformInfo["Scale"] = scale;
	//}

	{
		Json::Value mesh;
		mesh.append(_mesh->GetGUIName());
	}

}

void MeshRenderer::DeSerializer(Json::Value& scene)
{
}

void MeshRenderer::SetMaterial(Ref<Material> material, int32 index)
{
	if (_materials.size() <= static_cast<size_t>(index)) {
		_materials.resize(static_cast<size_t>(index + 1));
	}

	_materials[index] = material;
}

uint64 MeshRenderer::GetInstanceID()
{
	// 다 있을경우에만 Instancing을 할 수 있다는 전제조건으로 만들어야한다.
	if (_mesh == nullptr || _materials.empty()) {
		return 0;
	}

	// 같은 의미
	//uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ _mesh->GetID(), _materials[0]->GetID()};
	return instanceID.id;
}