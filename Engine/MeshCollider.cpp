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

}

void MeshCollider::FinalUpdate()
{
	auto transform = GetGameObject()->GetTransform();

	// 만약 같다면 수정사항이 없으므로 넘어간다.
	if (m_Center == transform->GetWorldPosition() && m_Scale == transform->localScale) {
		return;
	}

	m_Scale = transform->localScale;
	m_Center = transform->GetWorldPosition();

	vector<Vertex>& vertexs = GetGameObject()->GetMeshRenderer()->mesh->GetVertices();
	vector<uint32>& indexs = GetGameObject()->GetMeshRenderer()->mesh->GetIndices();

	m_BoundMesh.clear();

	Vec3 maxScale = Vec3::Zero;
	int32 size = indexs.size();
	for (int32 i = 0; i < size; i += 3) {
		m_BoundMesh.push_back(ColliderVertex{
			m_Center + vertexs[indexs[i]].pos * m_Scale,
			m_Center + vertexs[indexs[i + 1]].pos,
			m_Center + vertexs[indexs[i + 2]].pos * m_Scale });

		int32 index = m_BoundMesh.size() - 1;
		maxScale.x = max(maxScale.x, m_BoundMesh[index].pos0.x);
		maxScale.z = max(maxScale.z, m_BoundMesh[index].pos0.z);

		maxScale.x = max(maxScale.x, m_BoundMesh[index].pos1.x);
		maxScale.z = max(maxScale.z, m_BoundMesh[index].pos1.z);

		maxScale.x = max(maxScale.x, m_BoundMesh[index].pos2.x);
		maxScale.z = max(maxScale.z, m_BoundMesh[index].pos2.z);
	}

	m_Bound.Center = m_Center;
	m_Bound.Center.x += maxScale.x;
	m_Bound.Center.z += maxScale.z;
	m_Bound.Extents = maxScale;
}

bool MeshCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	float dist = 0.0f;
	if (m_Bound.Intersects(rayOrigin, rayDir, dist) == false) {
		return false;
	}

	for (const ColliderVertex& vertex : m_BoundMesh) {
		if (TriangleTests::Intersects(rayOrigin, rayDir,
			vertex.pos0 , vertex.pos1, vertex.pos2, distance)) {
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

		for (const ColliderVertex& vertex : m_BoundMesh) {
			if (box->BoundBox.Intersects(vertex.pos0, vertex.pos1, vertex.pos2)) {
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

		for (const ColliderVertex& vertex : m_BoundMesh) {
			if (box->BoundSphere.Intersects(vertex.pos0, vertex.pos1, vertex.pos2)) {
				return true;
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
