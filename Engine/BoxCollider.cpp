#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "MeshCollider.h"

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
	m_BoundBox.Extents = GetTransform()->localScale / 2;
	m_BoundBox.Orientation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(GetTransform()->localRotation.y, 
		GetTransform()->localRotation.x, GetTransform()->localRotation.z);

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
			if (m_BoundBox.Intersects(selfPos0, selfPos1, selfPos2)) {
				return true;
			}
		}
	}
	}
}

void BoxCollider::EditorUpdate()
{
	if (ImGui::CollapsingHeader("BoxCollider")) {
		ImGui::DragFloat3("Center", &m_Center.x, 0.1f, -1000.0f, 1000.0f);
		ImGui::DragFloat3("Extent", &m_BoundBox.Extents.x, 0.1f, 0.0f, 1000.0f);
	}
}
