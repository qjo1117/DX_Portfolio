#pragma once
#include "Component.h"

class Mesh;
class Material;


union InstanceID
{
	struct
	{
		uint32 meshID;
		uint32 materialID;
	};
	uint64 id;
};

union PickInfo
{
	// uint32 index값안에 각 바이트별로 정보를 추출해서 Color값으로
	// 저장을 해둔다.
	struct {
		char r;
		char g;
		char b;
		char a;
	};
	uint32 index;
};

/*------------------
	MeshRenderer
-------------------*/
class MeshRenderer : public Component
{
public:
	/* ----- Constructor Function ----- */
	MeshRenderer();
	virtual ~MeshRenderer();

public:
	/* ----- Helper Function ----- */
	void SetMesh(Ref<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(Ref<Material> material, int32 index = 0);

	Ref<Material> GetMaterial(int32 index = 0) {
		if (index > _materials.size()) { return nullptr; } 
		return _materials[index];
	}
	Ref<Mesh> GetMesh() { return _mesh; }

	uint32 GetMaterialSize() { return _materials.size(); }

	uint64 GetInstanceID();

	/* ----- virtual Function ----- */
	void Render();
	void Render(Ref<class InstancingBuffer>& buffer);
	void RenderShadow();
	void RenderPick(uint32 index);

	/* ----- Serializer Function ------ */
	virtual void Serializer(class Json::Value& object);
	virtual void DeSerializer(class Json::Value& object);

private:
	/* ----- MeshRender Variable ----- */
	Ref<Mesh>				_mesh;
	vector<Ref<Material>>	_materials;

	Ref<Material>			_shadow;
	Ref<Material>			_pick;

};

