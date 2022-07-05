#pragma once

#include "Component.h"

class Material;
class Mesh;
class StructuredBuffer;

struct ParticleInfo
{
	Vec3	worldPos;
	float	curTime;
	Vec3	worldDir;
	float	lifeTime;
	int32	alive;
	int32	padding[3];
};

struct ComputeShaderdInfo
{
	int32 addCount;
	int32 padding[3];
};

class ParticleSystem : public Component
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

public:
	virtual void FinalUpdate();
	virtual void EditorUpdate() override;
	void Render();

public:
	virtual void Load(const wstring& path) override {}
	virtual void Save(const wstring& path) override {}


public:
	Ref<class Texture>  GetTexture();

	void SetTexture(Ref<class Texture> tex);

private:
	Ref<StructuredBuffer>	_particleBuffer;
	Ref<StructuredBuffer>	_computeSharedBuffer;
	PRIVATE_PROPERTY(uint32, maxParticle) = 1000;

	Ref<Material>			_computeMaterial;
	Ref<Material>			_material;
	Ref<Mesh>				_mesh;

	PRIVATE_PROPERTY(float, createInterval)		= 0.005f;
	PRIVATE_PROPERTY(float, accTime)			= 0.0f;
	PRIVATE_PROPERTY(float, minLifeTime)		= 0.5f;
	PRIVATE_PROPERTY(float, maxLifeTime)		= 1.0f;
	PRIVATE_PROPERTY(float, minSpeed)			= 100.0f;
	PRIVATE_PROPERTY(float, maxSpeed)			= 50.0f;
	PRIVATE_PROPERTY(float, startScale)			= 10.0f;
	PRIVATE_PROPERTY(float, endScale)			= 5.0f;

};

