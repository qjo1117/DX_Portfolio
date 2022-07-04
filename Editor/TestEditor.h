#pragma once


#include "IEditor.h"


class TestEditor :
    public IEditor
{
public:

	TestEditor() {}
	virtual ~TestEditor() {}

	MENUITEM("Menu/Test", TestEditor, Register);
	static void Register()
	{
		m_pInstance = EDITOR->GetWindow<TestEditor>();
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

private:
	inline static TestEditor* m_pInstance = nullptr;
};

