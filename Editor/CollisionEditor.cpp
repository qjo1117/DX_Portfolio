#include "pch.h"
#include "CollisionEditor.h"

void CollisionEditor::Init()
{
	Title = "CollisionEditor";
}

void CollisionEditor::Show()
{
	static ImVec2 size(-1, -1);
	static ImVec2 offset(30.0f, 30.0f);
	ImGui::PushID(0);
	ImGui::BeginGroup(); // Lock X position

	ImGui::InvisibleButton("##empty", size);
	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		offset.x += ImGui::GetIO().MouseDelta.x;
		offset.y += ImGui::GetIO().MouseDelta.y;
	}

	const ImVec2 p0 = ImGui::GetItemRectMin();
	const ImVec2 p1 = ImGui::GetItemRectMax();
	const ImVec2 text_pos = ImVec2(p0.x + offset.x, p0.y + offset.y);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImGui::PushClipRect(p0, p1, true);

	draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
	draw_list->AddLine(ImVec2{ offset.x + 25.0f, offset.y + 25.0f }, ImVec2{ offset.x + 165.0f,offset.y + 50.0f }, IM_COL32_WHITE);
	draw_list->AddRectFilled(ImVec2{0.0f + offset.x,0.0f + offset.y}, ImVec2{ 50.0f + offset.x,50.0f + offset.y}, IM_COL32_WHITE);
	draw_list->AddRectFilled(ImVec2{130.0f + offset.x,0.0f + offset.y}, ImVec2{ 200.0f + offset.x,80.0f + offset.y}, IM_COL32_WHITE);

	ImGui::PopClipRect();

	ImGui::EndGroup();
	ImGui::PopID();
}

void CollisionEditor::End()
{
}
