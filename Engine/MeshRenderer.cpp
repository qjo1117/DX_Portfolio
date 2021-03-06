#include "pch.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "InstancingBuffer.h"
#include "Resources.h"
#include "Animator.h"
#include "EditorManager.h"

MeshRenderer::MeshRenderer() :
	Component(COMPONENT_TYPE::MESH_RENDERER)
{
	m_shadow = GET_SINGLE(Resources)->Get<Material>(L"Shadow");
	m_pick = GET_SINGLE(Resources)->Get<Material>(L"Pick");

}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Render()
{
	for (uint32 i = 0; i < m_materials.size(); ++i) {
		Ref<Material>& material = m_materials[i];

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
		m_mesh->Render(1, i);
	}
}

void MeshRenderer::Render(Ref<InstancingBuffer>& buffer)
{
	for (uint32 i = 0; i < m_materials.size(); ++i) {
		Ref<Material>& material = m_materials[i];

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
		m_mesh->Render(buffer, i);
	}
}

void MeshRenderer::EditorUpdate()
{
	if (_gameObject.lock()->GetTerrain() != nullptr) {
		return;
	}

	if (ImGui::CollapsingHeader("MeshRenderer")) {
		if (ImGui::Button("Shadow")) {
			_gameObject.lock()->isShadow = !_gameObject.lock()->isShadow;
		}

		ImGui::Text("Mesh : %s", Utils::Wstr2Str(m_mesh->name).data());
		ImGui::SameLine();
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_DRAG")) {
				m_mesh = static_pointer_cast<Mesh>(EDITOR->PickObject);
				EDITOR->PickObject = nullptr;
			}
			ImGui::EndDragDropTarget();
		}

		/* ----- Material ----- */
		string text;
		int32 size = GetMaterialSize();
		for (uint32 i = 0; i < size; ++i) {
			Ref<Material> material = m_materials[i];

			if (material == nullptr) {
				text = "nullptr";
			}
			else {
				text = Utils::Wstr2Str(material->name);
			}

			ImGui::Separator();
			ImGui::Text("Material : %s", text.data());
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MATERIAL_DRAG")) {
					m_materials[i] = static_pointer_cast<Material>(EDITOR->PickObject);
					EDITOR->PickObject = nullptr;
				}
				ImGui::EndDragDropTarget();
			}

			/* ----- Shader ----- */
			ImGui::Text("Shader : %s", Utils::Wstr2Str(material->GetShader()->name).data());
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SHADER_DRAG")) {
					m_materials[i]->shader = static_pointer_cast<Shader>(EDITOR->PickObject);
					EDITOR->PickObject = nullptr;
				}
				ImGui::EndDragDropTarget();
			}

			/* ----- Texture ----- */

			text = "Albedo##" + to_string(i);
			if (ImGui::CollapsingHeader(text.data())) {
				if (material->GetTexture(0) == nullptr) {
					ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
					text = "nullptr";
				}
				else {
					ImGui::Image(material->GetTexture(0), ImVec2(50.0f, 50.0f));
					text = Utils::Wstr2Str(material->GetTexture(0)->name);
				}
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
						material->SetTexture(0, static_pointer_cast<Texture>(EDITOR->PickObject));
						EDITOR->PickObject = nullptr;
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				Vec4 color = material->GetVec4(0);
				text = "Color##" + to_string(i);
				ImGui::ColorEdit4(text.data(), &color.x);
				material->SetVec4(0, color);
			}

			text = "Normal##" + to_string(i);
			if (ImGui::CollapsingHeader(text.data())) {
				if (material->GetTexture(1) == nullptr) {
					ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
					text = "nullptr";
				}
				else {
					ImGui::Image(material->GetTexture(1), ImVec2(50.0f, 50.0f));
					text = Utils::Wstr2Str(material->GetTexture(1)->name);
				}
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
						material->SetTexture(1, static_pointer_cast<Texture>(EDITOR->PickObject));
						EDITOR->PickObject = nullptr;
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				ImGui::Text(text.data());
			}

			text = "Specular" + to_string(i);
			if (ImGui::CollapsingHeader(text.data())) {
				if (material->GetTexture(2) == nullptr) {
					ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
					text = "nullptr";
				}
				else {
					ImGui::Image(material->GetTexture(2), ImVec2(50.0f, 50.0f));
					text = Utils::Wstr2Str(material->GetTexture(2)->name);
				}
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
						material->SetTexture(2, static_pointer_cast<Texture>(EDITOR->PickObject));
						EDITOR->PickObject = nullptr;
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				ImGui::Text(text.data());
			}
		}
	}

}

void MeshRenderer::RenderShadow()
{
	GetTransform()->PushData();
	m_shadow->PushGraphicsData();
	m_mesh->Render();
}

void MeshRenderer::RenderPick(uint32 index)
{
	PickInfo info;
	info.index = index;
	m_pick->SetVec4(0, Vec4(info.r, info.g, info.b, info.a));

	GetTransform()->PushData();
	m_pick->PushGraphicsData();
	m_mesh->Render();
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

	/*{
		Json::Value mesh;
		mesh.append(m_mesh->name());
	}*/

}

void MeshRenderer::DeSerializer(Json::Value& scene)
{
}

void MeshRenderer::SetMaterial(Ref<Material> material, int32 index)
{
	if (m_materials.size() <= static_cast<size_t>(index)) {
		m_materials.resize(static_cast<size_t>(index + 1));
	}

	m_materials[index] = material;
}

uint64 MeshRenderer::GetInstanceID()
{
	// ?? ???????????? Instancing?? ?? ?? ?????? ???????????? ????????????.
	if (m_mesh == nullptr || m_materials.empty()) {
		return 0;
	}

	// ???? ????
	//uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ m_mesh->id, m_materials[0]->id };
	return instanceID.id;
}