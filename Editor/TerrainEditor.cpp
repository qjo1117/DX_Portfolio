#include "pch.h"
#include "TerrainEditor.h"
#include "Terrain.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "Terrain.h"

void TerrainEditor::Init()
{

}

void TerrainEditor::Show()
{
	if (ImGui::Button("Brush")) {
		m_type = BRUSH_TYPE::END;
	}

	if (ImGui::DragFloat("Speed", &m_speed, 0.1f, 0.1f, 100.0f)) {

	}

	if (INPUT->GetButton(KEY_TYPE::LBUTTON) == false) {
		return;
	}

	Ref<Camera> camera = GET_SINGLE(SceneManager)->GetCurrentScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix matProj = camera->matProjection;

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * INPUT->GetMousePos().x / width - 1.0f) / matProj(0, 0);
	float viewY = (-2.0f * INPUT->GetMousePos().y / height + 1.0f) / matProj(1, 1);

	Matrix matView = camera->matView;
	Matrix matViewInv = matView.Invert();

	Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

	rayOrigin = XMVector3TransformCoord(rayOrigin, matViewInv);
	rayDir = XMVector3TransformNormal(rayDir, matViewInv);
	rayDir.Normalize();

	float minDistance = FLT_MAX;
	Ref<GameObject> picked;

	RayCastHitInfo hit;
	if (GET_SINGLE(ColliderManager)->RayCast(rayOrigin, rayDir, OUT hit, LAYER_TYPE::GROUND, 10000.0f)) {
		picked = hit.collider->GetGameObject();
		vector<Vertex> vertexs = picked->GetMeshRenderer()->mesh->GetVertices();

		Vec3 scale = picked->GetTransform()->localScale;
		Vec3 pos = picked->GetTransform()->GetWorldPosition();
		hit.point.y = 0.0f;

		for (auto& vertex : vertexs) {
			Vec3 subPos = Vec3{ pos.x + vertex.pos.x * scale.x, 0.0f , pos.z + vertex.pos.z * scale.z };
			Vec3 origin = hit.point - subPos;
			
			if (origin.x * origin.x + origin.z * origin.z < m_radius * m_radius) {
				vertex.pos.y += m_speed * DELTATIME;
			}
		}

		picked->GetMeshRenderer()->mesh->Modify(vertexs);
	}


	if (GET_SINGLE(ColliderManager)->RayCast(rayOrigin, rayDir, OUT hit, LAYER_TYPE::GROUND, 10000.0f)) {
		//picked = hit.collider->GetGameObject();

		//D3D12_RANGE range{0,0};
		//
		//ScratchImage& image =  picked->GetTerrain()->GetheightMap()->GetImage();

		///* ----- Resource 데이터를 GPU전용데이터로 만들어 줄 준비를 한다. ----- */
		//vector<D3D12_SUBRESOURCE_DATA> subResources;

		//::PrepareUpload(
		//	DEVICE.Get(),
		//	image.GetImages(),
		//	image.GetImageCount(),
		//	image.GetMetadata(),
		//	subResources
		//);

		//uint8* pData = (uint8*)subResources[0].pData;
		//for (int32 i = 0; i < subResources[0].RowPitch; ++i) {
		//	pData[i] = 0;
		//}

		//const uint64 bufferSize = ::GetRequiredIntermediateSize(picked->GetTerrain()->GetheightMap()->GetTex2D().Get(), 0, static_cast<uint32>(subResources.size()));

		//D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		//D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, D3D12_RESOURCE_FLAG_NONE);

		//ComPtr<ID3D12Resource> textureUploadHeap;
		//DEVICE->CreateCommittedResource(
		//	&heapProperty,
		//	D3D12_HEAP_FLAG_NONE,
		//	&desc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(textureUploadHeap.GetAddressOf())
		//);

		//
		///* ----- SubResource를 업데이트해서 정보를 갱신합니다. ----- */
		//::UpdateSubresources(
		//	RESOURCE_CMD_LIST.Get(),
		//	picked->GetTerrain()->GetheightMap()->GetTex2D().Get(),
		//	textureUploadHeap.Get(),
		//	0,
		//	0,
		//	static_cast<uint32>(subResources.size()),
		//	subResources.data()
		//);

		//GEngine->GetGraphicsCmdQueue()->FlushResourceCommandQueue();
	}

}

void TerrainEditor::End()
{
}
