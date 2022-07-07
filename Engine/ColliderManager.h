#pragma once

#include "BaseCollider.h"
#include "OctorTree.h"

enum class COLLIDER_STATE
{
	ENTER,
	PRESS,
	LEAVE,
	END
};

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

private:
	PRIVATE_PROPERTY(vector<Ref<ColliderInfo>>, Collider);
	PRIVATE_PROPERTY(Ref<OctorTree>, Tree);


	PRIVATE_PROPERTY(Ref<class Mesh>, cubeMesh);
	PRIVATE_PROPERTY(Ref<class Mesh>, sphereMesh);
	PRIVATE_PROPERTY(Ref<class Material>, material);
};

