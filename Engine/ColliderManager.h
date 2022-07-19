#pragma once

#include "BaseCollider.h"
#include "OcTree.h"

class ColliderManager
{
public:
	DECLARE_SINGLE(ColliderManager)

	void Init();
	void Update();
	void Render();
	void End();

	void AddCollider(Ref<BaseCollider> collider);

	bool RayCast(Vec3 rayOrin, Vec3 rayDir, OUT RayCastHitInfo& hit, LAYER_TYPE layer = LAYER_TYPE::END, float maxDistance = FLT_MAX);
	bool RayCastSphere(Vec3 rayOrin, Vec3 rayDir, float radius, OUT RayCastHitInfo& hit, LAYER_TYPE layer = LAYER_TYPE::END, float maxDistance = FLT_MAX);
	bool RayCastBox(Vec3 rayOrin, Vec3 rayDir, Vec3 extents, OUT RayCastHitInfo& hit, LAYER_TYPE layer = LAYER_TYPE::END, float maxDistance = FLT_MAX);
private:
	bool RayCastToColliders(OUT Vec4& rayOrigin, Vec4 rayDir, OUT float& distance, OUT RayCastHitInfo& hit, LAYER_TYPE layer, float maxDistance);

	vector<Ref<BaseCollider>> GetLayerObject(LAYER_TYPE layer);

private:
	PRIVATE_PROPERTY(vector<Ref<BaseCollider>>, Collider);
	PRIVATE_PROPERTY(Ref<OcTree>, Tree);

	PRIVATE_PROPERTY(Ref<class Mesh>, cubeMesh);
	PRIVATE_PROPERTY(Ref<class Mesh>, sphereMesh);
	PRIVATE_PROPERTY(Ref<class Material>, material);
	PRIVATE_PROPERTY(bool, isRender);

	mutex m;
};

