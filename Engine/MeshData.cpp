#include "pch.h"
#include "MeshData.h"
#include "FBXLoader.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "PathManager.h"
#include "DirectoryManager.h"

MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

Ref<MeshData> MeshData::LoadFromFBX(const wstring& path)
{
	Ref<MeshData> meshData = make_shared<MeshData>();

	uint32 index = static_cast<uint32>(path.find(L".fbx"));

	for (; path[index] != '\\'; --index) {}
	const wchar_t* name = &path[index + 1];

	vector<wstring> vecName = Utils::Split(name, '.');

	vector<Ref<FileInfo>> vecFileInfo;
	FBXLoader loader;
	bool isLoader = false;
	GET_SINGLE(DirectoryManager)->FindFileInfo(GET_SINGLE(DirectoryManager)->GetFileInfo(), vecFileInfo, ".json");

	Ref<MeshData> data = make_shared<MeshData>();
	//for (auto& fileInfo : vecFileInfo) {
	//	if (Utils::Str2Wstr(fileInfo->Name) == vecName[0] + L".json") {
	//		data->Load(GET_SINGLE(PathManager)->FindPath(FBX_PATH_KEY) + vecName[0]);
	//		isLoader = true;
	//	}
	//}

	if (isLoader == true) {
		loader.LoadFbxHasAnimation(path);
		for (int32 i = 0; i < loader.GetMeshCount(); ++i) {
			MeshRenderInfo info = {};
			info.mesh = Mesh::CreateFromFBX_HasAnimation(&loader.GetMesh(i), loader, data->_mesh);
			info.mesh->name = path;
			GET_SINGLE(Resources)->Add<Mesh>(name + std::to_wstring(i), info.mesh);

			for (size_t j = 0; j < loader.GetMesh(i).materials.size(); ++j) {
				Ref<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
				info.materials.push_back(material);
			}
			meshData->_meshRenders.push_back(info);
		}

	}
	else {
		loader.LoadFbx(path);
		for (int32 i = 0; i < loader.GetMeshCount(); ++i) {
			MeshRenderInfo info = {};
			info.mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader);
			info.mesh->name = path;
			GET_SINGLE(Resources)->Add<Mesh>(name + std::to_wstring(i), info.mesh);

			for (size_t j = 0; j < loader.GetMesh(i).materials.size(); ++j) {
				Ref<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
				info.materials.push_back(material);
			}
			meshData->_meshRenders.push_back(info);
		}
	}

	return meshData;
}

void MeshData::Load(const wstring& path)
{
	std::ifstream file(wstring(path + L".json"));

	Json::Value json;		// 매쉬 정보를 입력해줄 Json 객체

	/* ----- Vertices Save ------ */
	Json::CharReaderBuilder builder;
	JSONCPP_STRING errs;
	if (parseFromStream(builder, file, &json, &errs) == false) {
		return;
	}

	vector<BoneInfo> vecBoneInfo;
	int32 boneSize = json["BoneCount"].asInt();
	for (int32 i = 0; i < boneSize; ++i) {
		BoneInfo info;
		Utils::FromJson(json["Bone"][i], info);
		vecBoneInfo.push_back(info);
	}

	vector<AnimClipInfo> vecAnimClip;
	int32 animSize = json["AnimCount"].asInt();
	for (int32 i = 0; i < animSize; ++i) {
		AnimClipInfo info;
		Utils::FromJson(json["Anim"][i], info);
		vecAnimClip.push_back(info);
	}

	file.close();

	_mesh = make_shared<Mesh>();
	_mesh->SetAnimationClip(vecAnimClip);
	_mesh->SetBoneInfo(vecBoneInfo);
}

void MeshData::Save(const wstring& path)
{
	std::ofstream file(path + L".json");

	Json::Value json;

	for (MeshRenderInfo& info : _meshRenders) {
		if (info.mesh->IsAnimMesh() == true) {
			json["BoneCount"] = info.mesh->GetBoneInfos().size();
			for (auto& bone : info.mesh->GetBoneInfos()) {
				Json::Value vecJson;
				Utils::ToJson(vecJson, bone);
				json["Bone"].append(vecJson);
			}
		}

		if (info.mesh->IsAnimMesh() == true) {
			json["AnimCount"] = info.mesh->GetAnimClips().size();
			for (auto& clip : info.mesh->GetAnimClips()) {
				Json::Value vecJson;
				Utils::ToJson(vecJson, clip);
				json["Anim"].append(vecJson);
			}
		}
	}
	Json::StreamWriterBuilder builder;
	unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(json, &file);

 	file.close();
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
