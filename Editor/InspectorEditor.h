#pragma once

#include "IEditor.h"

class InspectorEditor :
    public IEditor
{
public:
	InspectorEditor() {}
	virtual ~InspectorEditor() {}

	MENUITEM("Menu/Inspector", InspectorEditor, Register);
	static void Register()
	{
		m_pInstance = EDITOR->GetWindow<InspectorEditor>();
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

public:
	static InspectorEditor* GetI() { return m_pInstance; }

private:
	inline static InspectorEditor* m_pInstance = nullptr;
};

