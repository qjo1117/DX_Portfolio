#pragma once

#include "Object.h"

class Mesh;
class Material;
class GameObject;

struct MeshRenderInfo
{
	Ref<Mesh> mesh;
	vector<Ref<Material>> materials;
};

class MeshData : public Object
{
public:
	MeshData();
	virtual ~MeshData();

public:
	static Ref<MeshData> LoadFromFBX(const wstring& path);

	virtual void Load(const wstring& path) override;
	virtual void Save(const wstring& path) override;

	vector<Ref<GameObject>> Instantiate();

private:
	Ref<Mesh>				_mesh;
	vector<Ref<Material>>	_materials;

	vector<MeshRenderInfo>	_meshRenders;
};

