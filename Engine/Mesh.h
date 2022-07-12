#pragma once

#include "Object.h"


struct IndexBufferInfo
{
	ComPtr<ID3D12Resource>		buffer;
	vector<uint32>				indices;
	D3D12_INDEX_BUFFER_VIEW		bufferView;
	DXGI_FORMAT					format;
	uint32						count;
};

class StructuredBuffer;

/*---------
	Mesh
----------*/
class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh();
public:
	/* ----- External Function ----- */
	void Create(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexbuffer);
	void Render(uint32 instanceCount = 1, uint32 index = 0);
	void Render(Ref<class InstancingBuffer>& buffer, uint32 index = 0);

	static Ref<Mesh> CreateFromFBX(const struct FbxMeshInfo* meshInfo, class FBXLoader& loader);

	void Modify(const vector<Vertex>& vertexBuffer);
	void Modify(const vector<uint32>& indexBuffer, int32 index = 0);
	void Modify(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer);

public:
	/* ----- Serializer Function ------ */
	void Save(const wstring& path) override;
	void Load(const wstring& path) override;

public:
	/* ----- Helper Function ----- */
	vector<Vertex>& GetVertices() { return _vertices; }
	vector<uint32>& GetIndices() { return _vecIndexInfo[0].indices; }

private:
	/* ----- Create Function ----- */
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);
	void CreateBonesAndAnimations(class FBXLoader& loader);
	Matrix GetMatrix(FbxAMatrix& matrix);

public:
	uint32 GetSubsetCount() { return static_cast<uint32>(_vecIndexInfo.size()); }
	const vector<BoneInfo>* GetBones() { return &_bones; }
	uint32 GetBoneCount() { return static_cast<uint32>(_bones.size()); }
	const vector<AnimClipInfo>* GetAnimClip() { return &_animClips; }

	bool IsAnimMesh() { return !_animClips.empty(); }
	Ref<StructuredBuffer> GetBoneFrameDataBuffer(int32 index = 0) { return _frameBuffer[index]; }
	Ref<StructuredBuffer> GetBoneOffsetBuffer() { return _offsetBuffer; }

	ComPtr<ID3D12Resource>	GetVertexBuffer() { return _vertexBuffer; }
private:
	/* ----- Vertex Variable ----- */
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32						_vertexCount = 0;

	vector<Vertex>				_vertices;

	/* ----- Indices Variable ----- */
	vector<IndexBufferInfo>		_vecIndexInfo;

	/* ----- Animation ----- */
	vector<AnimClipInfo>			_animClips;
	vector<BoneInfo>				_bones;

	/* ----- Animation Data ----- */
	Ref<StructuredBuffer>			_offsetBuffer; // 각 뼈의 offset 행렬
	vector<Ref<StructuredBuffer>>	_frameBuffer; // 전체 본 프레임 정보

};

