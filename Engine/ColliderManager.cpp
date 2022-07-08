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
	m_Tree->Init(4096.0f, 4096.0f, 4096.0f);			// ³ÐÀº ¹üÀ§·Î °Ë»çÇÑ´Ù.

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

		for (auto& dest : vecCheck) {
			if (collider->Collider.lock()->Collision(dest) == true) {
				if (collider->State.Enter == false && collider->State.Press == false) {
					collider->State.Enter = true;
				}
				else if (collider->State.Enter == true && collider->State.Press == false) {
					collider->State.Enter = false;
					collider->State.Press = true;
				}
			}

			if (collider->State.Enter == true) {
				for (function<void(Ref<BaseCollider>)> func : collider->Collider.lock()->BindEnterFunc) {
					func(dest);
				}
			}
			else if (collider->State.Press == true) {
				for (function<void(Ref<BaseCollider>)> func : collider->Collider.lock()->BindPressFunc) {
					func(dest);
				}
			}
		}

		if(vecCheck.size() == 0) {
			if (collider->State.Enter == true || collider->State.Press == true) {
				collider->State.Enter = false;
				collider->State.Press = false;
				collider->State.Leave = true;
			}
			else {
				collider->State.Leave = false;
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
			
			if (collider->State.Enter == true || collider->State.Press == true) {
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
		{
			Ref<SphereCollider> sphere = dynamic_pointer_cast<SphereCollider>(collider->Collider.lock());
			Ref<Transform> transform = collider->Collider.lock()->GetTransform();

			transform->PushData();

			if (collider->State.Enter == true || collider->State.Press == true) {
				m_material->SetInt(0, 1);
			}
			else {
				m_material->SetInt(0, 0);
			}
			m_material->PushGraphicsData();
			m_sphereMesh->Render(1, 0);
		}
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

bool ColliderManager::RayCast(Vec3 rayOrin, Vec3 rayDir, OUT RayCastHit& hit, float maxDistance)
{
	Vec4 origin = Vec4{ rayOrin.x, rayOrin.y, rayOrin.z, 1.0f };		// ±¤¼±À» ½î´Â À§Ä¡
	Vec4 direction = Vec4{ rayDir.x, rayDir.y, rayDir.z, 1.0f };		// ±¤¼±ÀÇ ¹æÇâ


	if (RayCastToColliders(origin, direction, hit.distance, maxDistance) == true) {
		hit.point = Vec3{ origin.x,origin.y,origin.z };
		return true;
	}

	return false;
}

bool ColliderManager::RayCastToColliders(OUT Vec4& rayOrigin, Vec4 rayDir, OUT float& distance, float maxDistance)
{
	float dist = rayDir.LengthSquared();
	float temp = 0;
	for (float dir = 0.0f; dir <= maxDistance; dir += dist) {
		for (auto& collider : m_Collider) {
			if (collider->Collider.lock()->Intersects(rayOrigin, rayDir, OUT temp)) {
				if (temp > maxDistance) {
					continue;
				}
				distance = dir;
				return true;
			}
		}
		rayOrigin += rayDir;
	}
	return false;
}
