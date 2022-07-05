#include "pch.h"
#include "MeshData.h"
#include "FBXLoader.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "PathManager.h"

MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

Ref<MeshData> MeshData::LoadFromFBX(const wstring& path)
{
	Ref<MeshData> meshData = make_shared<MeshData>();

	FBXLoader loader;
	loader.LoadFbx(path);

	uint32 index = static_cast<uint32>(path.find(L".fbx"));

	for (; path[index] != '\\'; --index) { }
	const wchar_t* name = &path[index + 1];

	for (int32 i = 0; i < loader.GetMeshCount(); ++i) {
		MeshRenderInfo info = {};
		info.mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader);
		info.mesh->SetName(path);
		GET_SINGLE(Resources)->Add<Mesh>(name + std::to_wstring(i), info.mesh);

		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); ++j) {
			Ref<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			info.materials.push_back(material);
		}
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

void MeshData::Load(const wstring& path)
{
	// TODO : 일단 이 함수가 실행되면 중지하고 생각하자.
	assert(nullptr);

	// TODO
	std::ifstream file(path + _name + FILE_EXPENSION);

	_mesh->Load(path);

	for (const Ref<Material> material : _materials) {
		material->Load(path);
	}

	for (const MeshRenderInfo& meshInfo : _meshRenders) {

	}

	file.close();
}

void MeshData::Save(const wstring& path)
{
	

	// TODO
}

vector<Ref<GameObject>> MeshData::Instantiate()
{
	vector<Ref<GameObject>> vec;

	for (MeshRenderInfo& info : _meshRenders) {
		Ref<GameObject> go = make_shared<GameObject>();

		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<MeshRenderer>());
		go->GetMeshRenderer()->mesh = info.mesh;

		for (uint32 i = 0; i < info.materials.size(); i++) {
			go->GetMeshRenderer()->SetMaterial(info.materials[i], i);
		}

		if (info.mesh->IsAnimMesh()) {
			Ref<Animator> animator = make_shared<Animator>();
			go->AddComponent(animator);
			animator->SetBones(info.mesh->GetBones());
			animator->SetAnimClip(info.mesh->GetAnimClip());
		}

		vec.push_back(go);
	}

	return vec;
}
