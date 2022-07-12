#include "pch.h"
#include "HierarchyEditor.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

#include "SceneManager.h"

void HierarchyEditor::Init()
{
	m_objects.reserve(1000);
	Title = "HierarchyEditor";
}

void HierarchyEditor::Show()
{
	Clear();

	// Editor Object Index Change
	vector<Ref<GameObject>>& objects = GET_SINGLE(SceneManager)->GetCurrentScene()->GetAllGameObjects();
	for (int32 i = 0; i < objects.size(); ++i) {
		if (objects[i]->GetTransform()->GetParent().lock() != nullptr) {
			continue;
		}
		ParentTree(objects[i]);
	}

}

void HierarchyEditor::End()
{
	Clear();
}

void HierarchyEditor::Create()
{

}

void HierarchyEditor::Clear()
{
	m_objects.clear();
}

void HierarchyEditor::ParentTree(const Ref<GameObject>& obj)
{
	uint32 node_flags = 0;
	m_strName = Utils::Wstr2Str(obj->name) + "##" + std::to_string(obj->id);

	if (obj->GetTransform()->GetChilds().size() == 0) {
		node_flags |= ImGuiTreeNodeFlags_Leaf; // ImGuiTreeNodeFlags_Bullet
		if (ImGui::TreeNodeEx(m_strName.data(), node_flags)) {
			if (ImGui::IsItemActivated()) {
				PickObject = obj;
			}
			ImGui::TreePop();
		}
	}
	else {

		if (ImGui::TreeNodeEx(m_strName.data(), node_flags)) {
			if (ImGui::IsItemActivated()) {
				PickObject = obj;
			}
			for (WRef<Transform>& child : obj->GetTransform()->GetChilds()) {
				ParentTree(child.lock()->GetGameObject());
			}

			ImGui::TreePop();
		}
	}
}
