#include "pch.h"
#include "InspectorEditor.h"
#include "HierarchyEditor.h"

void InspectorEditor::Init()
{
	Title = "InspectorEditor";
}

void InspectorEditor::Show()
{
	if (HierarchyEditor::GetI() == nullptr) {
		return;
	}
	if (HierarchyEditor::GetI()->PickObject.lock() == nullptr) {
		return;
	}

	Ref<GameObject> go = HierarchyEditor::GetI()->PickObject.lock();

	static char strTemp[50] = {};
	::sprintf_s(strTemp, "%s", Utils::Wstr2Str(go->name));

	ImGui::Checkbox("##Check", &go->isActive);
	ImGui::SameLine();
	static char _text[MAX_PATH / 2] = { 0 };
	::strcpy_s(_text, Utils::Wstr2Str(go->name).data());
	ImGui::InputText("##Nmae", _text, MAX_PATH / 2);
	go->name = Utils::Str2Wstr(_text);
	ImGui::SameLine();

	string layer = EnumToStr((LAYER_TYPE)go->layerType);				// 매크로 사용
	::sprintf_s(strTemp, 50, "/ Layer : %s", layer.data());
	ImGui::Text(strTemp);

	// 이름이 길으닌깐 맵핑
	Ref<GameObject> pick = HierarchyEditor::GetI()->PickObject.lock();

	vector<Ref<Component>> components = pick->GetAllComponent();

	for (Ref<Component>& component : components) {
		component->EditorUpdate();
	}

}

void InspectorEditor::End()
{
}
