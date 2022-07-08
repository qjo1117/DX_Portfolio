#pragma once

#include "BaseCollider.h"
#include "OctorTree.h"



struct ColliderState
{
public:
	bool Enter = false;
	bool Press = false;
	bool Leave = false;
};

struct ColliderInfo
{
	WRef<BaseCollider> Collider;
	ColliderState State = {};
};

class ColliderManager
{
public:
	DECLARE_SINGLE(ColliderManager);


	void Init();
	void Update();
	void Render();

	void AddCollider(Ref<BaseCollider> collider);

	bool RayCast(Vec3 rayOrin, Vec3 rayDir, OUT RayCastHit& hit, float maxDistance);
private:
	bool RayCastToColliders(OUT Vec4& rayOrigin, Vec4 rayDir, OUT float& distance, float maxDistance);

private:
	PRIVATE_PROPERTY(vector<Ref<ColliderInfo>>, Collider);
	PRIVATE_PROPERTY(Ref<OctorTree>, Tree);

	PRIVATE_PROPERTY(Ref<class Mesh>, cubeMesh);
	PRIVATE_PROPERTY(Ref<class Mesh>, sphereMesh);
	PRIVATE_PROPERTY(Ref<class Material>, material);
};

