#include "pch.h"
#include "PluginEditor.h"
#include "DirectoryManager.h"
#include "PluginManager.h"

void PluginEditor::Init()
{

}

void PluginEditor::Show()
{
	for(const auto& plugin : GET_SINGLE(PluginManager)->Plugins()) {
		const wstring& name = plugin.first;

		ImGui::Text(Utils::Wstr2Str(name).data());
	}

	ImGui::Separator();

	if (ImGui::Button("Load") == true) {
		GET_SINGLE(PluginManager)->LoadPlugins();
	}

}

void PluginEditor::End()
{
}
