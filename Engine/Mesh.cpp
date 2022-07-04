#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Material.h"
#include "InstancingBuffer.h"
#include "FBXLoader.h"
#include "StructuredBuffer.h"
#include "PathManager.h"

Mesh::Mesh() : Object(OBJECT_TYPE::MESH)
{
}

Mesh::~Mesh()
{
}

void Mesh::Create(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

void Mesh::Render(uint32 instanceCount, uint32 index)
{
	/* ----- CommandQueue에서 어떤 형식으로 어떤 버퍼를 사용할 것인지 정보를 전달한다.----- */
	GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)
	GRAPHICS_CMD_LIST->IASetIndexBuffer(&_vecIndexInfo[index].bufferView);

	// 2) TableDescHeap에다가 CBV 전달
	// 3) 모두 세팅이 끝났으면 TableDescHeap 커밋
	GEngine->GetGraphicsDescHeap()->CommitTable();

	GRAPHICS_CMD_LIST->DrawIndexedInstanced(_vecIndexInfo[index].count, instanceCount, 0, 0, 0);
}

void Mesh::Render(Ref<InstancingBuffer>& buffer, uint32 index)
{
	D3D12_VERTEX_BUFFER_VIEW bufferViews[] = { _vertexBufferView, buffer->GetBufferView() };
	GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 2, bufferViews);
	GRAPHICS_CMD_LIST->IASetIndexBuffer(&_vecIndexInfo[index].bufferView);

	GEngine->GetGraphicsDescHeap()->CommitTable();

	GRAPHICS_CMD_LIST->DrawIndexedInstanced(_vecIndexInfo[index].count, buffer->GetCount(), 0, 0, 0);
}

Ref<Mesh> Mesh::CreateFromFBX(const FbxMeshInfo* meshInfo, FBXLoader& loader)
{
	Ref<Mesh> mesh = make_shared<Mesh>();
	mesh->CreateVertexBuffer(meshInfo->vertices);

	for (const vector<uint32>& buffer : meshInfo->indices) {
		if (buffer.empty()) {		// 임시처리
			vector<uint32> defaultBuffer{ 0 };
			mesh->CreateIndexBuffer(defaultBuffer);
		}
		else {
			mesh->CreateIndexBuffer(buffer);
		}
	}

	if (meshInfo->hasAnimation) {
		mesh->CreateBonesAndAnimations(loader);
	}

	return mesh;
}

void Mesh::Save(const wstring& path)
{
	wstring pullPath = path + _name + FILE_EXPENSION;
	std::ofstream file(pullPath);

	Json::Value mesh;		// 매쉬 정보를 입력해줄 Json 객체
	
	/* ----- Vertices Save ------ */
	Json::Value verticesInfo;
	Json::Value vertex;
	verticesInfo["VerticesCount"] = _vertices.size();
	for (const Vertex& vertice : _vertices) {
		Json::Value info;
		Utils::JsonSaveToVertex(info, vertice);
		vertex.append(info);
	}
	verticesInfo["Position"] = vertex;

	/* ----- Indices Save ------ */
	Json::Value indicesInfo;
	Json::Value info;

	for (const IndexBufferInfo& indices : _vecIndexInfo) {
		for (const uint32& indice : indices.indices) {
			info.append(indice);
		}
	}

	indicesInfo["IndicesCount"] = info.size();
	indicesInfo["Indices"] = info;

	mesh["Vertex"] = verticesInfo;
	mesh["Indice"] = indicesInfo;

	Json::StreamWriterBuilder builder;
	unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(mesh, &file);

	file.close();
}

void Mesh::Load(const wstring& path)
{
	wstring pullPath = path + _name + FILE_EXPENSION;
	std::ifstream file(pullPath);

	Json::Value mesh;		// 매쉬 정보를 입력해줄 Json 객체

	/* ----- Vertices Save ------ */
	Json::CharReaderBuilder builder; 
	JSONCPP_STRING errs;
	if (parseFromStream(builder, file, &mesh, &errs) == false) {
		return;
	}

	Json::Value verticesInfo = mesh["Vertex"];
	Json::Value indicesInfo = mesh["Indice"];

	vector<Vertex> vertexs;
	{
		int32 size = verticesInfo["VerticesCount"].asInt();
		Json::Value iterator = verticesInfo["Position"];
		for (Json::ValueIterator iter = iterator.begin(); iter != iterator.end(); iter++) {
			Vertex vertex;
			Utils::JsonLoadToVertex(*iter, vertex);
			vertexs.push_back(vertex);
		}
	}

	vector<uint32> indices;
	{
		int32 size = indicesInfo["IndicesCount"].asInt();
		Json::Value iterator = indicesInfo["Indices"];
		for (Json::ValueIterator iter = iterator.begin(); iter != iterator.end(); iter++) {
			indices.push_back(iter->asInt());
		}
	}

	Create(vertexs, indices);

	file.close();
}


void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	/* ----- VertexBuffer를 만들어준다. ----- */
	_vertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	// 1) Buffer에다가 데이터 세팅
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	_vertices.reserve(_vertexCount);
	for (const Vertex& vertex : buffer) {
		_vertices.push_back(vertex);
	}

	/* ----- GPU에 어떤 크기를 사용할 것이고 크기는 어떤지 알려준다. ----- */
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	/* ----- IndicesBuffer를 만들어준다. ----- */
	uint32 indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = indexCount * sizeof(uint32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> indexBuffer;
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	// 1) Buffer에다가 데이터 세팅
	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	indexBuffer->Unmap(0, nullptr);

	/* ----- GPU에 어떤 크기를 사용할 것이고 크기는 어떤지 알려준다. ----- */
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = bufferSize;

	IndexBufferInfo info = {
		indexBuffer,
		buffer,
		indexBufferView,
		DXGI_FORMAT_R32_UINT,
		indexCount
	};
	_vecIndexInfo.push_back(info);
}

void Mesh::CreateBonesAndAnimations(FBXLoader& loader)
{


#pragma region AnimClip
	uint32 frameCount = 0;
	vector<Ref<FbxAnimClipInfo>>& animClips = loader.GetAnimClip();
	for (Ref<FbxAnimClipInfo>& ac : animClips)
	{
		AnimClipInfo info = {};

		info.animName = ac->name;
		info.duration = ac->endTime.GetSecondDouble() - ac->startTime.GetSecondDouble();

		int32 startFrame = static_cast<int32>(ac->startTime.GetFrameCount(ac->mode));
		int32 endFrame = static_cast<int32>(ac->endTime.GetFrameCount(ac->mode));
		info.frameCount = endFrame - startFrame;

		info.keyFrames.resize(ac->keyFrames.size());

		const int32 boneCount = static_cast<int32>(ac->keyFrames.size());
		for (int32 b = 0; b < boneCount; b++)
		{
			auto& vec = ac->keyFrames[b];

			const int32 size = static_cast<int32>(vec.size());
			frameCount = max(frameCount, static_cast<uint32>(size));
			info.keyFrames[b].resize(size);

			for (int32 f = 0; f < size; f++)
			{
				FbxKeyFrameInfo& kf = vec[f];
				// FBX에서 파싱한 정보들로 채워준다
				KeyFrameInfo& kfInfo = info.keyFrames[b][f];
				kfInfo.time = kf.time;
				kfInfo.frame = static_cast<int32>(size);
				kfInfo.scale.x = static_cast<float>(kf.matTransform.GetS().mData[0]);
				kfInfo.scale.y = static_cast<float>(kf.matTransform.GetS().mData[1]);
				kfInfo.scale.z = static_cast<float>(kf.matTransform.GetS().mData[2]);

				kfInfo.rotation.x = static_cast<float>(kf.matTransform.GetQ().mData[0]);
				kfInfo.rotation.y = static_cast<float>(kf.matTransform.GetQ().mData[1]);
				kfInfo.rotation.z = static_cast<float>(kf.matTransform.GetQ().mData[2]);
				kfInfo.rotation.w = static_cast<float>(kf.matTransform.GetQ().mData[3]);

				kfInfo.translate.x = static_cast<float>(kf.matTransform.GetT().mData[0]);
				kfInfo.translate.y = static_cast<float>(kf.matTransform.GetT().mData[1]);
				kfInfo.translate.z = static_cast<float>(kf.matTransform.GetT().mData[2]);
			}
		}

		_animClips.push_back(info);
	}
#pragma endregion

#pragma region Bones
	vector<Ref<FbxBoneInfo>>& bones = loader.GetBones();
	for (Ref<FbxBoneInfo>& bone : bones)
	{
		BoneInfo boneInfo = {};
		boneInfo.parentIdx = bone->parentIndex;
		boneInfo.matOffset = GetMatrix(bone->matOffset);
		boneInfo.boneName = bone->boneName;
		_bones.push_back(boneInfo);
	}
#pragma endregion

#pragma region SkinData
	if (IsAnimMesh())
	{
		// BoneOffet 행렬
		const int32 boneCount = static_cast<int32>(_bones.size());
		vector<Matrix> offsetVec(boneCount);
		for (size_t b = 0; b < boneCount; b++)
			offsetVec[b] = _bones[b].matOffset;

		// OffsetMatrix StructuredBuffer 세팅
		_offsetBuffer = make_shared<StructuredBuffer>();
		_offsetBuffer->Init(sizeof(Matrix), static_cast<uint32>(offsetVec.size()), offsetVec.data());

		const int32 animCount = static_cast<int32>(_animClips.size());
		for (int32 i = 0; i < animCount; i++)
		{
			AnimClipInfo& animClip = _animClips[i];

			// 애니메이션 프레임 정보
			vector<AnimFrameParams> frameParams;
			frameParams.resize(_bones.size() * animClip.frameCount);

			for (int32 b = 0; b < boneCount; b++)
			{
				const int32 keyFrameCount = static_cast<int32>(animClip.keyFrames[b].size());
				for (int32 f = 0; f < keyFrameCount; f++)
				{
					int32 idx = static_cast<int32>(boneCount * f + b);

					if (idx > frameParams.size()) {
						break;
					}

					frameParams[idx] = AnimFrameParams
					{
						Vec4(animClip.keyFrames[b][f].scale),
						animClip.keyFrames[b][f].rotation, // Quaternion
						Vec4(animClip.keyFrames[b][f].translate)
					};
				}
			}

			// StructuredBuffer 세팅
			_frameBuffer.push_back(make_shared<StructuredBuffer>());
			_frameBuffer.back()->Init(sizeof(AnimFrameParams), static_cast<uint32>(frameParams.size()), frameParams.data());
		}
	}
#pragma endregion
}

Matrix Mesh::GetMatrix(FbxAMatrix& matrix)
{
	Matrix mat;

	for (int32 i = 0; i < 4; ++i) {
		for (int32 j = 0; j < 4; ++j) {
			mat.m[i][j] = static_cast<float>(matrix.Get(i, j));
		}
	}

	return mat;
}

