#pragma once

#include "IEditor.h"

class ResourceEditor :
    public IEditor
{
public:
	ResourceEditor() {}
	virtual ~ResourceEditor() {}

	MENUITEM("Menu/Resource", ResourceEditor, Register);
	static void Register()
	{
		m_pInstance = EDITOR->GetWindow<ResourceEditor>();
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

public:
	static ResourceEditor* GetI() { return m_pInstance; }

private:
	inline static ResourceEditor* m_pInstance = nullptr;
};

