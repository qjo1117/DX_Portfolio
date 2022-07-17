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

	// 빌드 시작되면 검출을 시작
	if (m_isReBuild == true) {
		ImGui::Text("Loading");
		vector<Ref<FileInfo>> fileInfos;
		// 디렉토리를 조사해서 Txt가 있는지 확인
		GET_SINGLE(DirectoryManager)->FindFileInfo(GET_SINGLE(DirectoryManager)->GetFileInfo(), fileInfos, ".txt");
		if (fileInfos.size() != 0) {
			for (Ref<FileInfo>& info : fileInfos) {
				// 찾게 된다면 다시 로드
				if (info->Name == "Rebuild.txt") {
					GET_SINGLE(PluginManager)->LoadPlugins();

					m_isReBuild = false;
					GET_SINGLE(DirectoryManager)->isCheck = false;
				}
			}
		}
	}

	if (ImGui::Button("Complier") == true) {
		GET_SINGLE(PluginManager)->UnLoadPlugins();
		system("D:/Cpp/DirectX12/DX_Portfolio/Build.bat");
		m_isReBuild = true;
		GET_SINGLE(DirectoryManager)->isCheck = true;
	}

}

void PluginEditor::End()
{
}
