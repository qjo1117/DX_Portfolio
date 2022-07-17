#include "pch.h"
#include "Terrain.h"
#include "Resources.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "Transform.h"

#include "Input.h"
#include "EditorManager.h"
#include "Engine.h"

Terrain::Terrain() : Component(COMPONENT_TYPE::TERRAIN)
{
}

Terrain::~Terrain()
{
}

void Terrain::Init(int32 sizeX, int32 sizeZ)
{
	m_sizeX = sizeX;
	m_sizeZ = sizeZ;

	m_material = GET_SINGLE(Resources)->Get<Material>(L"Terrain");
	m_mesh = GET_SINGLE(Resources)->Get<Mesh>(L"Terrain");

	m_material->SetInt(1, m_sizeX);
	m_material->SetInt(2, m_sizeZ);
	m_material->SetFloat(0, m_maxTesselation);

	m_heightMap = GET_SINGLE(Resources)->Get<Texture>(L"HeightMap_0");
	Vec2 vec = Vec2(m_heightMap->GetWidth(), m_heightMap->GetHeight());

	vector<D3D12_SUBRESOURCE_DATA> subResources;

	HRESULT hr = ::PrepareUpload(
		DEVICE.Get(),
		m_heightMap->GetImage().GetImages(),
		m_heightMap->GetImage().GetImageCount(),
		m_heightMap->GetImage().GetMetadata(),
		subResources
	);


	uint8* pointer = (uint8*)subResources[0].pData;
	for (int32 i = 1; i < subResources[0].RowPitch; i += 3) {
		m_Height.push_back(*(pointer + i));
	}

	// ----------------------------------------------------------------



	m_material->SetVec2(0, Vec2(m_heightMap->GetWidth(), m_heightMap->GetHeight()));
	m_material->SetVec2(1, Vec2(1000.0f, 5000.0f));
	m_material->SetTexture(2, m_heightMap);

	Ref<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	meshRenderer->mesh = (GET_SINGLE(Resources)->LoadTerrainMesh(sizeX, sizeZ));
	meshRenderer->SetMaterial(m_material);
	
	GetGameObject()->AddComponent(meshRenderer);
}

void Terrain::Update()
{

}

void Terrain::FinalUpdate()
{
	Ref<Camera> mainCamera = GET_SINGLE(SceneManager)->GetCurrentScene()->GetMainCamera();
	if (mainCamera == nullptr) {
		return;
	}

	Vec3 cameraPos = mainCamera->GetTransform()->localPosition;
	m_material->SetVec4(0, Vec4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f));

}

void Terrain::EditorUpdate()
{
	string guiName = "";

	if (ImGui::CollapsingHeader("Terrain")) {
		if (ImGui::CollapsingHeader("Property")) {

		}

		if (ImGui::CollapsingHeader("Base")) {
			if (m_material->GetTexture(0) == nullptr) {
				ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(m_material->GetTexture(0), ImVec2(50.0f, 50.0f));
				guiName = Utils::Wstr2Str(m_material->GetTexture(0)->name);
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					if (EDITOR->PickObject->name == L"Default") {
						m_material->SetTexture(0, nullptr);
					}
					else {
						m_material->SetTexture(0, static_pointer_cast<Texture>(EDITOR->PickObject));
					}
					EDITOR->PickObject = nullptr;
				}
				ImGui::EndDragDropTarget();
			}


		}
		if (ImGui::CollapsingHeader("Normal")) {
			if (m_material->GetTexture(1) == nullptr) {
				ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(m_material->GetTexture(1), ImVec2(50.0f, 50.0f));
				guiName = Utils::Wstr2Str(m_material->GetTexture(1)->name);
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					if (EDITOR->PickObject->name == L"Default") {
						m_material->SetTexture(1, nullptr);
					}
					else {
						m_material->SetTexture(1, static_pointer_cast<Texture>(EDITOR->PickObject));
					}
					EDITOR->PickObject = nullptr;
				}
				ImGui::EndDragDropTarget();
			}


		}
		if (ImGui::CollapsingHeader("HeightMap")) {
			if (m_material->GetTexture(2) == nullptr) {
				ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(m_material->GetTexture(2), ImVec2(50.0f, 50.0f));
				guiName = Utils::Wstr2Str(m_material->GetTexture(2)->name);
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					if (EDITOR->PickObject->name == L"Default") {
						m_material->SetTexture(2, nullptr);
					}
					else {
						m_material->SetTexture(2, static_pointer_cast<Texture>(EDITOR->PickObject));
					}
					EDITOR->PickObject = nullptr;
				}
				ImGui::EndDragDropTarget();
			}
		}

	}
}
