#pragma once

#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

/*--------------
	Resources
---------------*/
class Resources
{
public:
	DECLARE_SINGLE(Resources)
	using KeyObjMap = std::map<wstring, Ref<Object>>;

public:
	void Init();
	void End();

	// TODO 각 Load, Save함수를 구현해야함.
	template<typename T>
	Ref<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	Ref<T> Save(const wstring& key, const wstring& path);

	void SaveObjectList(const wstring& path);
	void LoadObjectList(const wstring& path);

	template<typename T>
	bool Add(const wstring& key, Ref<T> object);

	template<typename T>
	Ref<T> Get(const wstring& key);

	template<typename T>
	OBJECT_TYPE GetObjectType();

	Ref<Mesh> LoadCubeMesh();
	Ref<Mesh> LoadSphereMesh();
	Ref<Mesh> LoadRectangleMesh();
	Ref<Mesh> LoadCircleMesh();
	Ref<Mesh> LoadCylinderMesh();
	Ref<Mesh> LoadConeMesh();
	Ref<Mesh> LoadPointMesh();
	Ref<Mesh> LoadTerrainMesh(int32 sizeX = 15, int32 sizeZ = 15);

	Ref<class MeshData> LoadFBX(const wstring& path);

	Ref<Texture> CreateTexture(const wstring& name, DXGI_FORMAT format, uint32 width, uint32 height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resFlags = D3D12_RESOURCE_FLAG_NONE, Vec4 clearColor = Vec4());

	Ref<Texture> CreateTextureFromResource(const wstring& name, ComPtr<ID3D12Resource> tex2D);

	KeyObjMap& GetResources(OBJECT_TYPE type) { return _resources[static_cast<uint32>(type)]; }
private:
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateDefaultGameObject();
	void CreateDefaultComponent();
	void CreateDefaultTexture();
	void CreateDefaultLoad();

private:

	array<KeyObjMap, OBJECT_TYPE_COUNT> _resources;

};

template<typename T>
inline Ref<T> Resources::Load(const wstring& key, const wstring& path)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end()) {
		return static_pointer_cast<T>(findIt->second);
	}

	Ref<T> object = make_shared<T>();
	object->SetName(key);
	object->Load(path);
	keyObjMap[key] = object;


	return object;
}

template<typename T>
bool Resources::Add(const wstring& key, Ref<T> object)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;
	object->SetName(key);

	return true;
}

template<typename T>
Ref<T> Resources::Get(const wstring& key)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template<typename T>
inline OBJECT_TYPE Resources::GetObjectType()
{
	if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::GAMEOBJECT;
	else if (std::is_same_v<T, Material>)
		return OBJECT_TYPE::MATERIAL;
	else if (std::is_same_v<T, Mesh>)
		return OBJECT_TYPE::MESH;
	else if (std::is_same_v<T, Shader>)
		return OBJECT_TYPE::SHADER;
	else if (std::is_same_v<T, Texture>)
		return OBJECT_TYPE::TEXTURE;
	else if (std::is_convertible_v<T, Component>)
		return OBJECT_TYPE::COMPONENT;
	else
		return OBJECT_TYPE::NONE;
}