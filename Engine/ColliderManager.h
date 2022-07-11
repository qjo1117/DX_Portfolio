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
	Ref<BaseCollider> Collider;
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

	bool RayCast(Vec3 rayOrin, Vec3 rayDir, OUT RayCastHitInfo& hit, LAYER_TYPE layer = LAYER_TYPE::END, float maxDistance = FLT_MAX);
private:
	bool RayCastToColliders(OUT Vec4& rayOrigin, Vec4 rayDir, OUT float& distance, LAYER_TYPE layer, float maxDistance);

	vector<Ref<ColliderInfo>> GetLayerObject(LAYER_TYPE layer);

private:
	PRIVATE_PROPERTY(vector<Ref<ColliderInfo>>, Collider);
	PRIVATE_PROPERTY(Ref<OctorTree>, Tree);

	PRIVATE_PROPERTY(Ref<class Mesh>, cubeMesh);
	PRIVATE_PROPERTY(Ref<class Mesh>, sphereMesh);
	PRIVATE_PROPERTY(Ref<class Material>, material);
	PRIVATE_PROPERTY(bool, isRender);

	mutex m;
};

