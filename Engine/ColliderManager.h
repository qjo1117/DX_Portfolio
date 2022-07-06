#pragma once

#include "BaseCollider.h"
#include "OctorTree.h"


class ColliderManager
{
public:
	DECLARE_SINGLE(ColliderManager);


	void Init();
	void Update();


private:
	PRIVATE_PROPERTY(vector<WRef<BaseCollider>>, Collider);
	PRIVATE_PROPERTY(Ref<OctorTree>, Tree);

};

