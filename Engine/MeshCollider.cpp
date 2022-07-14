#include "pch.h"
#include "MeshCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Mesh.h"

#include "BoxCollider.h"
#include "SphereCollider.h"


MeshCollider::MeshCollider() :
	BaseCollider(COLLIDER_TYPE::MESH)
{
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::Awake()
{
	vector<Vertex>& vertexs = GetGameObject()->GetMeshRenderer()->mesh->GetVertices();
	vector<uint32>& indexs = GetGameObject()->GetMeshRenderer()->mesh->GetIndices();

	int32 size = indexs.size();

	Vec3 scale;
	for (int32 i = 0; i < size - 2; i += 3) {
		Triangle triangle;
		triangle.pos0 = vertexs[indexs[i]].pos;
		triangle.pos1 = vertexs[indexs[i + 1]].pos;
		triangle.pos2 = vertexs[indexs[i + 2]].pos;
		m_BoundMesh.push_back(triangle);

		scale.x = max(triangle.pos0.x, max(triangle.pos1.x, triangle.pos2.x));
		scale.y = max(triangle.pos0.y, max(triangle.pos1.y, triangle.pos2.y));
		scale.z = max(triangle.pos0.z, max(triangle.pos1.z, triangle.pos2.z));
	}

	m_Bound.Center = GetTransform()->GetWorldPosition();
	m_Bound.Extents = scale;
	m_Scale = scale * GetTransform()->localScale;
}

void MeshCollider::FinalUpdate()
{
	auto transform = GetGameObject()->GetTransform();

	// 만약 같다면 수정사항이 없으므로 넘어간다.
	if (m_Center == transform->GetWorldPosition() && m_Scale == transform->localScale) {
		return;
	}

	m_Center = transform->GetWorldPosition();

	m_Bound.Center = m_Center;
	m_Bound.Extents = m_Scale;
}

bool MeshCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	float dist = 0.0f;
	if (m_Bound.Intersects(rayOrigin, rayDir, dist) == false) {
		return false;
	}

	for (const Triangle& vertex : m_BoundMesh) {
		Vec3 selfPos0 = m_Center + vertex.pos0 * m_Scale;
		Vec3 selfPos1 = m_Center + vertex.pos1 * m_Scale;
		Vec3 selfPos2 = m_Center + vertex.pos2 * m_Scale;
		if (TriangleTests::Intersects(rayOrigin, rayDir,
			selfPos0, selfPos1, selfPos2, distance)) {
			return true;
		}
	}
	return false;
}

bool MeshCollider::Collision(Ref<BaseCollider> collider)
{
	switch (collider->ColliderType) {
	case COLLIDER_TYPE::BOX:
	{
		Ref<BoxCollider> box = dynamic_pointer_cast<BoxCollider>(collider);
		float dist = 0.0f;
		if (m_Bound.Intersects(box->BoundBox) == false) {
			return false;
		}

		for (const Triangle& vertex : m_BoundMesh) {
			Vec3 scale = GetTransform()->localScale;

			Vec3 selfPos0 = m_Center + vertex.pos0 * scale;
			Vec3 selfPos1 = m_Center + vertex.pos1 * scale;
			Vec3 selfPos2 = m_Center + vertex.pos2 * scale;
			if (box->BoundBox.Intersects(selfPos0, selfPos1, selfPos2)) {
				return true;
			}
		}
	}
		break;

	case COLLIDER_TYPE::SPHERE:
	{
		Ref<SphereCollider> box = dynamic_pointer_cast<SphereCollider>(collider);
		float dist = 0.0f;
		if (m_Bound.Intersects(box->BoundSphere) == false) {
			return false;
		}

		for (const Triangle& vertex : m_BoundMesh) {
			Vec3 scale = GetTransform()->localScale;
			Vec3 selfPos0 = m_Center + vertex.pos0 * scale;
			Vec3 selfPos1 = m_Center + vertex.pos1 * scale;
			Vec3 selfPos2 = m_Center + vertex.pos2 * scale;
			if (box->BoundSphere.Intersects(selfPos0, selfPos1, selfPos2)) {
				return true;
			}
		}
	}
		break;
	case COLLIDER_TYPE::MESH:
	{
		Ref<MeshCollider> box = dynamic_pointer_cast<MeshCollider>(collider);
		float dist = 0.0f;
		if (m_Bound.Intersects(box->Bound) == false) {
			return false;
		}

		for (const Triangle& vertex : m_BoundMesh) {
			for (const Triangle& targetVertex : box->BoundMesh) {
				Vec3 scale = GetTransform()->localScale;
				Vec3 selfPos0 = m_Center + vertex.pos0 * scale;
				Vec3 selfPos1 = m_Center + vertex.pos1 * scale;
				Vec3 selfPos2 = m_Center + vertex.pos2 * scale;

				Vec3 target_Center = box->Center;

				Vec3 targetPos0 = target_Center + targetVertex.pos0 * scale;
				Vec3 targetPos1 = target_Center + targetVertex.pos1 * scale;
				Vec3 targetPos2 = target_Center + targetVertex.pos2 * scale;

				if (TriangleTests::Intersects(selfPos0, selfPos1, selfPos2,
					targetPos0, targetPos1, targetPos2) == true) {
					return true;
				}
			}
		}
	}
	break;
	}


	return false;
}

void MeshCollider::EditorUpdate()
{
}
