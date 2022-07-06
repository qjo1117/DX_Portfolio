#pragma once

#include "IEditor.h"

class HierarchyEditor :
    public IEditor
{
public:
	HierarchyEditor() {}
	virtual ~HierarchyEditor() {}

	MENUITEM("Menu/Hierarchy", HierarchyEditor, Register);
	static void Register()
	{
		if (m_pInstance == nullptr) {
			m_pInstance = EDITOR->GetWindow<HierarchyEditor>();
		}
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;


	void Create();
	void Clear();
	void ParentTree(const Ref<GameObject>& obj);

public:
	static HierarchyEditor* GetI() { return m_pInstance; }

private:
	inline static HierarchyEditor* m_pInstance = nullptr;

	vector<WRef<GameObject>> m_objects;
	string m_strName = "";
	PRIVATE_PROPERTY(WRef<GameObject>, PickObject);
};

