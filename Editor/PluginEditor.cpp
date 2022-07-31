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

		if (ImGui::Selectable(Utils::Wstr2Str(name).data()) == true) {
			m_strSelector = name;
		}
	}

	ImGui::Separator();

	if (ImGui::Button("Load") == true) {
		GET_SINGLE(PluginManager)->LoadPlugins();
	}
	if (ImGui::Button("UnLoad") == true) {
		GET_SINGLE(PluginManager)->UnLoadPlugin(m_strSelector);

	}

	if (ImGui::Button("Complier") == true) {
		GET_SINGLE(PluginManager)->UnLoadPlugins();

		vector<Ref<FileInfo>> fileInfos;
		GET_SINGLE(DirectoryManager)->FindFileInfo(GET_SINGLE(DirectoryManager)->GetFileInfo(), fileInfos, ".bat");
		system(fileInfos[0]->PathInfo.string().data());
		GET_SINGLE(PluginManager)->LoadPlugins();
		GET_SINGLE(PluginManager)->Start();
	}

}

void PluginEditor::End()
{
}
