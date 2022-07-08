#pragma once
#include "IEditor.h"

struct CollisionNode
{
public:
	ImVec2 pos = ImVec2{ 0.0f,0.0f };
	ImVec2 size = ImVec2{ 100.0f,40.0f };
	string name = "";
};

class CollisionEditor :
    public IEditor
{
public:
	CollisionEditor() {}
	virtual ~CollisionEditor() {}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

	MENUITEM("Menu/Collision", CollisionEditor, Register);
	static void Register()
	{
		if (m_pInstance == nullptr) {
			m_pInstance = EDITOR->GetWindow<CollisionEditor>();
		}
	}

private:
	inline static CollisionEditor* m_pInstance = nullptr;

	vector<CollisionNode> CollisionNode;
};

