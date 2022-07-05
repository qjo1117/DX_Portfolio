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
	int32 _sizeX = 15;
	int32 _sizeZ = 15;

	float _maxTesselation = 4.0f;

	Ref<class Material> _material;
	Ref<class Mesh> _mesh;
};

