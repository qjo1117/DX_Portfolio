#pragma once



class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

public:
	void LoadFbx(const wstring& path);
	void LoadFbxHasAnimation(const wstring& path);

public:
	int32 GetMeshCount() { return static_cast<int32>(_meshes.size()); }
	const FbxMeshInfo& GetMesh(int32 idx) { return _meshes[idx]; }
	vector<Ref<FbxBoneInfo>>& GetBones() { return _bones; }
	vector<Ref<FbxAnimClipInfo>>& GetAnimClip() { return _animClips; }
private:
	void	Import(const wstring& path);

	void	ParseNode(FbxNode* root);
	void	LoadMesh(FbxMesh* mesh);
	void	LoadMaterial(FbxSurfaceMaterial* surfaceMaterial);

	void	GetNormal(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	void	GetTangent(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	void	GetUV(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	Vec4	GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName);
	wstring	GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty);

	void	CreateTextures();
	void	CreateMaterials();

	// Animation
	void		LoadBones(FbxNode* node) { LoadBones(node, 0, -1); }
	void		LoadBones(FbxNode* node, int32 idx, int32 parentIdx);
	void		LoadAnimationInfo();

	void		LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo);
	void		LoadBoneWeight(FbxCluster* cluster, int32 boneIdx, FbxMeshInfo* meshInfo);
	void		LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* meshInfo);
	void		LoadKeyframe(int32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* container);

	int32		FindBoneIndex(string name);
	FbxAMatrix	GetTransform(FbxNode* node);

	void		FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo);

private:
	FbxManager*		_manager = nullptr;
	FbxScene*		_scene = nullptr;
	FbxImporter*	_importer = nullptr;
	wstring			_resourceDirectory;
	wstring			_path;

	vector<FbxMeshInfo>				_meshes;
	vector<Ref<FbxBoneInfo>>		_bones;
	vector<Ref<FbxAnimClipInfo>>	_animClips;
	FbxArray<FbxString*>			_animNames;
};
