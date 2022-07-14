#include "pch.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"

#include "BoxCollider.h"
#include "MeshCollider.h"

SphereCollider::SphereCollider() : BaseCollider(COLLIDER_TYPE::SPHERE)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::FinalUpdate()
{
	auto transform = GetGameObject()->GetTransform();

	m_BoundSphere.Center = transform->GetWorldPosition();
	Vec3 scale = transform->GetLocalScale();
	m_BoundSphere.Radius = max(scale.x, max(scale.y, scale.z)) / 2.0f;

	m_Bound.Center = m_BoundSphere.Center;
	m_Bound.Extents = Vec3::One * m_BoundSphere.Radius;
}

bool SphereCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return BoundSphere.Intersects(rayOrigin, rayDir, OUT distance);
}

bool SphereCollider::Collision(Ref<BaseCollider> collider)
{
	switch (collider->ColliderType) {
	case COLLIDER_TYPE::SPHERE:
		return BoundSphere.Intersects(dynamic_pointer_cast<SphereCollider>(collider)->BoundSphere);
		break;

	case COLLIDER_TYPE::BOX:
		return BoundSphere.Intersects(dynamic_pointer_cast<BoxCollider>(collider)->BoundBox);
		break;

	case COLLIDER_TYPE::MESH:
	{
		Ref<MeshCollider> mesh = dynamic_pointer_cast<MeshCollider>(collider);
		float dist = 0.0f;
		if (m_Bound.Intersects(mesh->Bound) == false) {
			return false;
		}

		for (const Triangle& vertex : mesh->BoundMesh) {
			Vec3 selfPos0 = mesh->Center + vertex.pos0 * mesh->Scale;
			Vec3 selfPos1 = mesh->Center + vertex.pos1 * mesh->Scale;
			Vec3 selfPos2 = mesh->Center + vertex.pos2 * mesh->Scale;
			if (m_BoundSphere.Intersects(selfPos0, selfPos1, selfPos2)) {
				return true;
			}
		}
	}
		break;
	}

	return false;
}

void SphereCollider::EditorUpdate()
{
	if (ImGui::CollapsingHeader("SphereCollider")) {
		ImGui::DragFloat("Radius", &_radius, 0.01f, 0.0f, 1000.0f);
	}
}
