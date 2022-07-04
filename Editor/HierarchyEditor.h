#pragma once

#include "IEditor.h"

class HierarchyEditor :
    public IEditor
{
public:
	HierarchyEditor() {}
	virtual ~HierarchyEditor() {}

	MENUITEM("Menu/Test", HierarchyEditor, Register);
	static void Register()
	{
		m_pInstance = EDITOR->GetWindow<HierarchyEditor>();
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

private:
	inline static HierarchyEditor* m_pInstance = nullptr;
};

