#include "pch.h"
#include "ColliderManager.h"
#include "Engine.h"

void ColliderManager::Init()
{
	m_Tree = make_shared<OctorTree>();
	m_Tree->Init(4096.0f, 4096.0f, 4096.0f);			// 넓은 범위로 검사한다.
}

void ColliderManager::Update()
{
	m_Tree->Clear();
	
	for (WRef<BaseCollider> collider : m_Collider) {
		if (collider.lock()->GetGameObject()->GetActive()) {
			m_Tree->Insert(collider.lock());
		}
	}
	
	for (WRef<BaseCollider>& collider : m_Collider) {
		vector<Ref<BaseCollider>> vecCheck;
		Octor octor = Octor{ collider.lock()->Bound.Center,collider.lock()->Bound.Extents };
		m_Tree->QuarryRange(octor, vecCheck);
	
		auto findIt = std::find(vecCheck.begin(), vecCheck.end(), collider.lock());
		if (findIt != vecCheck.end()) {
			vecCheck.erase(findIt);
		}
	
		for (auto& dest : vecCheck) {
			if (collider.lock()->Collision(dest) == true) {
				// 바인딩된 함수 호출
				for (function<void(Ref<BaseCollider>)> func : collider.lock()->BindFunc) {
					func(dest);
				}
			}
	
		}
	}
}
