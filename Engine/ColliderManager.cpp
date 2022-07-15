#include "pch.h"
#include "ColliderManager.h"

#include "Engine.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "MeshRenderer.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "MeshCollider.h"



void ColliderManager::Init()
{
	m_Tree = make_shared<OcTree>();
	m_Tree->Init(10000.0f, 10000.0f, 10000.0f);			// 넓은 범위로 검사한다.

	m_cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
	m_sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
	m_material = GET_SINGLE(Resources)->Get<Material>(L"Collider");
}

void ColliderManager::Update()
{
	m_Tree->Clear();
	
	// 처음 시작할때 트리를 재구성합니다.
	for (Ref<BaseCollider> collider : m_Collider) {
		if (collider->GetGameObject()->isActive == true) {
			if (m_Tree->Insert(collider) == false) {
				assert("Error");
			}
			collider->Check = false;
		}
	}
	
	// 그리고 콜라이더를 순회합니다.
	for (Ref<BaseCollider> collider : m_Collider) {
		if (collider->Check == true) {
			continue;
		}

		// 트리에서 큰 범위로써의 충돌검사를 진행합니다.
		vector<Ref<BaseCollider>> vecCheck;
		m_Tree->QuarryRange(collider, vecCheck);

		// 큰 범위로 충돌체크된 콜라이더를 각각 세부적으로 충돌체크를 진행합니다.
		for (Ref<BaseCollider> target : vecCheck) {
			// 충돌이 되었을 경우 각 상태를 정의합니다.
			if (collider->Collision(target) == true) {
				if (collider->State.Enter == false && collider->State.Press == false) {
					collider->State.Enter = true;
					collider->State.Press = false;
					target->State.Enter = true;
					target->State.Press = false;

					collider->Check = true;
					target->Check = true;
				}
				else if (collider->State.Enter == true && collider->State.Press == false) {
					collider->State.Enter = false;
					collider->State.Press = true;
					target->State.Enter = false;
					target->State.Press = true;

					collider->Check = true;
					target->Check = true;
				}
			}

			// 충돌이 되었을 경우 각 상태에 맞게 함수를 호출합니다.
			if (collider->State.Enter == true) {
				for (function<void(Ref<BaseCollider>)> func : collider->BindEnterFunc) {
					func(target);
				}
			}
			else if (collider->State.Press == true) {
				for (function<void(Ref<BaseCollider>)> func : collider->BindPressFunc) {
					func(target);
				}
			}

			if (target->State.Enter == true) {
				for (function<void(Ref<BaseCollider>)> func : target->BindEnterFunc) {
					func(collider);
				}
			}
			else if (target->State.Press == true) {
				for (function<void(Ref<BaseCollider>)> func : target->BindPressFunc) {
					func(collider);
				}
			}
		}

		if(vecCheck.size() == 0 && collider->Check == false) {
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
	if (m_isRender == false) {
		return;
	}

	for (auto collider : m_Collider) {
		switch (collider->ColliderType) {
		case COLLIDER_TYPE::BOX:
		{
			Ref<Transform> transform = collider->GetTransform();

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
			Ref<Transform> transform = collider->GetTransform();

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

		case COLLIDER_TYPE::MESH:
		{
			Ref<BaseCollider> mesh = collider;
			Ref<Transform> transform = collider->GetTransform();

			transform->PushData();

			if (collider->State.Enter == true || collider->State.Press == true) {
				m_material->SetInt(0, 1);
			}
			else {
				m_material->SetInt(0, 0);
			}
			m_material->PushGraphicsData();
			mesh->GetGameObject()->GetMeshRenderer()->mesh->Render();
		}
			break;
		}
	}
}

void ColliderManager::End()
{
	m_Tree->Clear();
	for (Ref<BaseCollider> collider : m_Collider) {
		collider = nullptr; // REF_RELASE
	}
}

void ColliderManager::AddCollider(Ref<BaseCollider> collider)
{
	lock_guard<mutex> lockGuard(m);
	m_Collider.push_back(collider);
}

bool ColliderManager::RayCast(Vec3 rayOrin, Vec3 rayDir, OUT RayCastHitInfo& hit, LAYER_TYPE layer, float maxDistance)
{
	Vec4 origin = Vec4{ rayOrin.x, rayOrin.y, rayOrin.z, 1.0f };		// 광선을 쏘는 위치
	Vec4 direction = Vec4{ rayDir.x, rayDir.y, rayDir.z, 1.0f };		// 광선의 방향

	if (RayCastToColliders(origin, direction, hit.distance, hit, layer, maxDistance) == true) {
		return true;
	}

	return false;
}

bool ColliderManager::RayCastToColliders(OUT Vec4& rayOrigin, Vec4 rayDir, OUT float& distance, OUT RayCastHitInfo& hit, LAYER_TYPE layer, float maxDistance)
{
	float dist = rayDir.LengthSquared();
	float temp = 0;

	vector<Ref<BaseCollider>> colliders = GetLayerObject(layer);

	for (float dir = 0.0f; dir <= maxDistance; dir += dist) {
		for (auto collider : colliders) {
			if (collider->Intersects(rayOrigin, rayDir, OUT temp)) {
				if (temp > maxDistance) {
					continue;
				}
				distance = dir;
				hit.collider = collider;
				hit.distance = dir;
				hit.normal = -rayDir;
				hit.point = rayOrigin + rayDir * temp;
				return true;
			}
		}
		rayOrigin += rayDir;
	}
	return false;
}

vector<Ref<BaseCollider>> ColliderManager::GetLayerObject(LAYER_TYPE layer)
{
	vector<Ref<BaseCollider>> listObject;

	if (static_cast<uint32>(layer) == static_cast<uint32>(LAYER_TYPE::END)) {
		return m_Collider;
	}

	for (Ref<BaseCollider> collider : m_Collider) {
		if (collider->GetGameObject()->layerType == static_cast<uint32>(layer)) {
			listObject.push_back(collider);
		}
	}

	return listObject;
}
