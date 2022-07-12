#include "pch.h"
#include "DirectoryEditor.h"
#include "DirectoryManager.h"

void DirectoryEditor::Init()
{
}

void DirectoryEditor::Show()
{
	Ref<FileInfo> root = GET_SINGLE(DirectoryManager)->GetFileInfo();

	Ref_TreeNode(root);

}

void DirectoryEditor::End()
{

}

void DirectoryEditor::Ref_TreeNode(Ref<FileInfo> info)
{
	for (auto& fileInfo : info->File) {
		if (fileInfo->Type == FILE_TYPE::FOLDER) {
			if (ImGui::TreeNode(fileInfo->Name.c_str())) {
				Ref_TreeNode(fileInfo);
				ImGui::TreePop();
			}
		}
		else {
			if (ImGui::Selectable(fileInfo->Name.c_str())) {

			}
		}
	}
}
