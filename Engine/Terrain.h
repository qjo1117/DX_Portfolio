#pragma once

#include "Component.h"

class Terrain : public Component
{
public:
	Terrain();
	virtual ~Terrain();

	void Init(int32 sizeX, int32 sizeZ);

	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void EditorUpdate() override;

private:
	PRIVATE_PROPERTY(int32, sizeX) = 15;
	PRIVATE_PROPERTY(int32, sizeZ) = 15;

	PRIVATE_PROPERTY(float, maxTesselation) = 4.0f;

	PRIVATE_PROPERTY(vector<float>, Height);

	PRIVATE_PROPERTY(Ref<class Material>, material);
	PRIVATE_PROPERTY(Ref<class Mesh>, mesh);
	PRIVATE_PROPERTY(Ref<class Texture>, heightMap);
};

