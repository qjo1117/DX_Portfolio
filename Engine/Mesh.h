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
	static Ref<Mesh> CreateFromFBX_HasAnimation(const struct FbxMeshInfo* meshInfo, class FBXLoader& loader, Ref<Mesh>& mesh);

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
	void CreateHasBonesAndAnimations();

private:
	/* ----- Create Function ----- */
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);
	void CreateBonesAndAnimations(class FBXLoader& loader);
	Matrix GetMatrix(FbxAMatrix& matrix);

public:
	void SetAnimationClip(vector<AnimClipInfo>& animClips) { m_animClips = animClips; }
	void SetBoneInfo(vector<BoneInfo>& bones) { m_bones = bones; }

	uint32 GetSubsetCount() { return static_cast<uint32>(_vecIndexInfo.size()); }
	const vector<BoneInfo>* GetBones() { return &m_bones; }
	uint32 GetBoneCount() { return static_cast<uint32>(m_bones.size()); }
	const vector<AnimClipInfo>* GetAnimClip() { return &m_animClips; }
	vector<AnimClipInfo>& GetAnimClips() { return m_animClips; }
	vector<BoneInfo>& GetBoneInfos() { return m_bones; }

	bool IsAnimMesh() { return !m_animClips.empty(); }
	Ref<StructuredBuffer> GetBoneFrameDataBuffer(int32 index = 0) { return m_frameBuffer[index]; }
	Ref<StructuredBuffer> GetBoneOffsetBuffer() { return m_offsetBuffer; }

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
	vector<AnimClipInfo>			m_animClips;
	vector<BoneInfo>				m_bones;

	/* ----- Animation Data ----- */
	Ref<StructuredBuffer>			m_offsetBuffer; // 각 뼈의 offset 행렬
	vector<Ref<StructuredBuffer>>	m_frameBuffer; // 전체 본 프레임 정보

};

