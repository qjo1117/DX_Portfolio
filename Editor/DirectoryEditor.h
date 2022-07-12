#pragma once

#ifndef EDITOR_DIRECTORY_H
#define EDITOR_DIRECTORY_H



class DirectoryEditor : public IEditor
{
public:
	DirectoryEditor() {}
	virtual ~DirectoryEditor() {}

	MENUITEM("Menu/Directory", DirectoryEditor, Present);
	static void Present()
	{
		m_pInstance = EDITOR->GetWindow<DirectoryEditor>();
		m_pInstance->Title = "Directory";
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

	void Ref_TreeNode(Ref<class FileInfo> info);

private:
	inline static DirectoryEditor* m_pInstance;
};

#endif