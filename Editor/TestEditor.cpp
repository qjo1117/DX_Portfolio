#include "pch.h"
#include "TestEditor.h"
#include "Resource.h"

void TestEditor::Init()
{
}

void TestEditor::Show()
{
	if (ImGui::Button("Hello")) {

	}

	ImGui::Image(GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(2), ImVec2{ 100.0f,100.0f });
	ImGui::Image(GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(0), ImVec2{ 100.0f,100.0f });
	ImGui::Image(GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(1), ImVec2{ 100.0f,100.0f });
	ImGui::Image(GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0), ImVec2{ 100.0f,100.0f });
	ImGui::Image(GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(0), ImVec2{ 100.0f,100.0f });


}

void TestEditor::End()
{
}
