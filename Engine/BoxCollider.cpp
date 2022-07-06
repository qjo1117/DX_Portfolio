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
	m_mesh = GET_SINGLE(Resources)->LoadCubeMesh();
	m_material = GET_SINGLE(Resources)->Get<Material>(L"Wirefram");
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::FinalUpdate()
{
	auto transform = GetGameObject()->GetTransform();

	m_BoundBox.Center = transform->GetWorldPosition();
	m_BoundBox.Extents = transform->localScale / 2;


	m_material->PushGraphicsData();
	m_mesh->Render();
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
		ImGui::DragFloat3("Center", &m_BoundBox.Center.x, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat3("Extent", &m_BoundBox.Extents.x, 0.01f, 0.0f, 1000.0f);
	}
}