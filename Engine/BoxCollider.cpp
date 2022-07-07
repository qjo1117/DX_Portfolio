#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"

BoxCollider::BoxCollider() : BaseCollider(COLLIDER_TYPE::BOX)
{


}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Awake()
{
	auto transform = GetGameObject()->GetTransform();
	m_BoundBox.Center = Vec3::Zero;
	m_BoundBox.Extents = transform->localScale / 2;
}

void BoxCollider::FinalUpdate()
{
	m_BoundBox.Center = GetTransform()->GetWorldPosition();
	m_BoundBox.Orientation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(GetTransform()->localRotation.x, GetTransform()->localRotation.y, GetTransform()->localRotation.z);

	m_Bound.Center = m_BoundBox.Center;
	m_Bound.Extents = m_BoundBox.Extents;

}

bool BoxCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return BoundBox.Intersects(rayOrigin, rayDir, OUT distance);
}

bool BoxCollider::Collision(Ref<BaseCollider> collider)
{
	switch (collider->ColliderType) {
	case COLLIDER_TYPE::SPHERE:
		return BoundBox.Intersects(dynamic_pointer_cast<SphereCollider>(collider)->BoundSphere);
		break;

	case COLLIDER_TYPE::BOX:
		return BoundBox.Intersects(dynamic_pointer_cast<BoxCollider>(collider)->BoundBox);
		break;
	}
}

void BoxCollider::EditorUpdate()
{
	if (ImGui::CollapsingHeader("BoxCollider")) {
		ImGui::DragFloat3("Center", &m_Center.x, 0.1f, -1000.0f, 1000.0f);
		ImGui::DragFloat3("Extent", &m_BoundBox.Extents.x, 0.1f, 0.0f, 1000.0f);
	}
}

bool BoxCollider::RayCast(Vec3 rayOrin, Vec3 rayDir, OUT RayCastHit& hit, float distance)
{
	Vec4 origin = Vec4{ rayOrin.x, rayOrin.y, rayOrin.z, 1.0f };		// ±¤¼±À» ½î´Â À§Ä¡
	Vec4 direction = Vec4{ rayDir.x, rayDir.y, rayDir.z, 1.0f };		// ±¤¼±ÀÇ ¹æÇâ
	float dist = direction.Length();

	bool check = false;
	for (float dir = 0.0f; dir <= distance; dir += 0.1f) {
		if (BoundBox.Intersects(origin, direction, OUT dist)) {
			check = true;
			break;
		}
		origin += direction;
	}

	

	return false;
}
