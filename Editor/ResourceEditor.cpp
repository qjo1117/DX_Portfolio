#include "pch.h"
#include "ResourceEditor.h"
#include "EditorManager.h"

void ResourceEditor::Init()
{
	Title = "ResourceEditor";
}

void ResourceEditor::Show()
{
	string guiName;

	if (ImGui::TreeNode("Texture")) {
		for (const pair<wstring, Ref<Object>>& tex : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::TEXTURE)) {
			guiName.assign(tex.first.begin(), tex.first.end());

			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("TEXTURE_DRAG", nullptr, 0);
				EDITOR->PickObject = tex.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Shader")) {
		for (const pair<wstring, Ref<Object>>& shader : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::SHADER)) {
			guiName.assign(shader.first.begin(), shader.first.end());

			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("SHADER_DRAG", nullptr, 0);
				EDITOR->PickObject = shader.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Material")) {
		for (const pair<wstring, Ref<Object>>& material : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::MATERIAL)) {
			guiName.assign(material.first.begin(), material.first.end());

			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("MATERIAL_DRAG", nullptr, 0);
				EDITOR->PickObject = material.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GameObject")) {
		for (const pair<wstring, Ref<Object>>& go : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::GAMEOBJECT)) {
			guiName.assign(go.first.begin(), go.first.end());

			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("GAMEOBJECT_DRAG", nullptr, 0);
				EDITOR->PickObject = go.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Compoenent")) {
		for (const pair<wstring, Ref<Object>>& component : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::COMPONENT)) {
			guiName.assign(component.first.begin(), component.first.end());

			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("COMPONENT_DRAG", nullptr, 0);
				EDITOR->PickObject = component.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Mesh")) {
		for (const pair<wstring, Ref<Object>>& mesh : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::MESH)) {
			guiName.assign(mesh.first.begin(), mesh.first.end());

			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("MESH_DRAG", nullptr, 0);
				EDITOR->PickObject = mesh.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}
}

void ResourceEditor::End()
{
}
