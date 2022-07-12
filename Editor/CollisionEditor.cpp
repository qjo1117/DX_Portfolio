#include "pch.h"
#include "CollisionEditor.h"
#include "OctoTree.h"
#include "ColliderManager.h"


void CollisionEditor::Init()
{
	Title = "CollisionEditor";
}

void CollisionEditor::Show()
{
	Clear();

	{
		lock_guard<mutex> lockGuard(m);
		SetNode(0, 0, GET_SINGLE(ColliderManager)->GetTree());
	}
	static ImVec2 size(-1, -1);
	static ImVec2 offset(0.0f, 0.0f);
	ImGui::PushID(0);
	ImGui::BeginGroup(); // Lock X position

	ImGui::InvisibleButton("##empty", size);
	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		offset.x += ImGui::GetIO().MouseDelta.x;
		offset.y += ImGui::GetIO().MouseDelta.y;
	}

	const ImVec2 p0 = ImGui::GetItemRectMin();
	const ImVec2 p1 = ImGui::GetItemRectMax();
	const ImVec2 text_pos = ImVec2(p0.x + offset.x, p0.y + offset.y);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImGui::PushClipRect(p0, p1, true);

	draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
	int32 vecSize = m_collisionNode.size();
	for(auto& node : m_collisionNode) {
		ImVec2 min = ImVec2{ node.pos.x - node.size.x + offset.x, node.pos.y - node.size.y + offset.y };
		ImVec2 max = ImVec2{ node.pos.x + node.size.x + offset.x, node.pos.y + node.size.y + offset.y };
		draw_list->AddRectFilled(min, max, IM_COL32_WHITE);

		draw_list->AddText(node.pos, ImColor(255, 0, 0, 0), node.name.data());

		//if (node.parentIndex == -1) {
		//	continue;
		//}

		//CollisionNode parent = m_collisionNode[node.parentIndex];

		//min = ImVec2{ node.pos.x - node.size.x + offset.x, node.pos.y - node.size.y + offset.y };
		//max = ImVec2{ parent.pos.x + parent.size.x + offset.x, parent.pos.y + parent.size.y + offset.y };
		//draw_list->AddLine(min, max, IM_COL32_WHITE);
	}

	ImGui::PopClipRect();

	ImGui::EndGroup();
	ImGui::PopID();
}

void CollisionEditor::End()
{
	Clear();
}

// 전체 순회후 Node추가
void CollisionEditor::SetNode(int32 level, int32 parent, Ref<OctoTree> octorTree)
{
	// 리스트에 정보가 없으면 리턴
	if (octorTree->vecList.size() == 0) {
		return;
	}

	// 노드 구성 
	int32 count = 1;
	for (Ref<BaseCollider> collider : octorTree->vecList) {
		CollisionNode data;
		data.name = Utils::Wstr2Str(collider->GetGameObject()->name);

		data.index = m_level;
		m_level += 1;

		data.pos.x = (data.size.x + 30.0f) * count;
		count += 1;

		data.pos.y = (data.size.y + 30.0f) * level;


		m_collisionNode.push_back(data);
	}

	count = 0;
	for (Ref<OctoTree> node : octorTree->GetChilds()) {
		if (node == nullptr) {
			return;
		}

		SetNode(level + 1, count, node);
		count += 1;
	}
}

void CollisionEditor::Clear()
{
	m_collisionNode.clear();
}
