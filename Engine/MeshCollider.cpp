#include "pch.h"
#include "MeshCollider.h"

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
}

bool MeshCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	for (const ColliderVertex& vertex : m_BoundMesh) {
		if (TriangleTests::Intersects(rayOrigin, rayDir,
			vertex.pos0, vertex.pos1, vertex.pos2, distance)) {
			return true;
		}
	}
	return false;
}

bool MeshCollider::Collision(Ref<BaseCollider> collider)
{

	return false;
}

void MeshCollider::EditorUpdate()
{
}
