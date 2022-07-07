#include "pch.h"
#include "ColliderManager.h"
#include "Engine.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

void ColliderManager::Init()
{
	m_Tree = make_shared<OctorTree>();
	m_Tree->Init(4096.0f, 4096.0f, 4096.0f);			// 넓은 범위로 검사한다.

	m_cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
	m_sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
	m_material = GET_SINGLE(Resources)->Get<Material>(L"Collider");
}

void ColliderManager::Update()
{
	m_Tree->Clear();
	
	for (auto collider : m_Collider) {
		if (collider->Collider.lock()->GetGameObject()->isActive == true) {
			m_Tree->Insert(collider->Collider.lock());
		}
	}
	
	for (auto& collider : m_Collider) {
		vector<Ref<BaseCollider>> vecCheck;
		m_Tree->QuarryRange(collider->Collider.lock(), vecCheck);
		collider->State.Enter = false;

		for (auto& dest : vecCheck) {
			if (collider->Collider.lock()->Collision(dest) == true) {
				collider->State.Enter = true;
				// 바인딩된 함수 호출
				for (function<void(Ref<BaseCollider>)> func : collider->Collider.lock()->BindFunc) {
					func(dest);
				}
			}
		}
	}
}

void ColliderManager::Render()
{
	for (auto& collider : m_Collider) {
		switch (collider->Collider.lock()->ColliderType) {
		case COLLIDER_TYPE::BOX:
		{
			Ref<BoxCollider> box = dynamic_pointer_cast<BoxCollider>(collider->Collider.lock());
			Ref<Transform> transform = collider->Collider.lock()->GetTransform();

			transform->PushData();
			
			if (collider->State.Enter == true) {
				m_material->SetInt(0, 1);
			}
			else {
				m_material->SetInt(0, 0);
			}
			m_material->PushGraphicsData();
			m_cubeMesh->Render(1, 0);
		}
			break;

		case COLLIDER_TYPE::SPHERE:

			break;
		}
	}
}

void ColliderManager::AddCollider(Ref<BaseCollider> collider)
{
	Ref<ColliderInfo> info = make_shared<ColliderInfo>();
	info->Collider = collider;
	m_Collider.push_back(info);
}
