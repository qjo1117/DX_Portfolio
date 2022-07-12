#pragma once
#include "IEditor.h"
class PluginEditor :
    public IEditor
{
public:
	PluginEditor() {}
	virtual ~PluginEditor() {}

	MENUITEM("Menu/Plugin", PluginEditor, Register);
	static void Register()
	{
		if (m_pInstance == nullptr) {
			m_pInstance = EDITOR->GetWindow<PluginEditor>();
		}
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

public:
	static PluginEditor* GetI() { return m_pInstance; }

private:
	inline static PluginEditor* m_pInstance = nullptr;
};

