#pragma once
#include "IEditor.h"

struct CollisionNode
{
public:
	int32 index = -1;
	ImVec2 pos = ImVec2{ 0.0f,0.0f };
	ImVec2 size = ImVec2{ 20.0f,7.0f};
	string name = "";

	int32 parentIndex = -1;
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

	//MENUITEM("Menu/Collision", CollisionEditor, Register);
	//static void Register()
	//{
	//	if (m_pInstance == nullptr) {
	//		m_pInstance = EDITOR->GetWindow<CollisionEditor>();
	//	}
	//}

	void SetNode(int32 level, int32 parent, Ref<OctorTree> octorTree);
	void Clear();

private:
	inline static CollisionEditor* m_pInstance = nullptr;

	int32 m_level = 0;
	vector<class CollisionNode> m_collisionNode;

	mutex m;
};

