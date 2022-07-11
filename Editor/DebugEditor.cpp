#include "pch.h"
#include "DebugEditor.h"
#include "Timer.h"

void DebugEditor::Init()
{
	Title = "DebugEditor";
}

void DebugEditor::Show()
{
	ImGui::Text("Frame : %d", GET_SINGLE(Timer)->GetFps());
	if (ImGui::Button("Collider")) {
		GET_SINGLE(ColliderManager)->isRender = !GET_SINGLE(ColliderManager)->isRender;
	}
}

void DebugEditor::End()
{
}
