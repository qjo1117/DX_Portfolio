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
}

void DebugEditor::End()
{
}
