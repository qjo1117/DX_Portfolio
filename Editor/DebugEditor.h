#pragma once


#include "IEditor.h"


class DebugEditor :
    public IEditor
{
public:
	DebugEditor() {}
	virtual ~DebugEditor() {}

	MENUITEM("Menu/Debug", DebugEditor, Register);
	static void Register()
	{
		if (m_pInstance == nullptr) {
			m_pInstance = EDITOR->GetWindow<DebugEditor>();
		}
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

public:
	DebugEditor* GetI() { return m_pInstance; }
private:
	inline static DebugEditor* m_pInstance = nullptr;
};

