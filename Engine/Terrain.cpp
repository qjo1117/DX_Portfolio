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

Terrain::Terrain() : Component(COMPONENT_TYPE::TERRAIN)
{
}

Terrain::~Terrain()
{
}

void Terrain::Init(int32 sizeX, int32 sizeZ)
{
	_sizeX = sizeX;
	_sizeZ = sizeZ;

	_material = GET_SINGLE(Resources)->Get<Material>(L"Terrain");
	_mesh = GET_SINGLE(Resources)->Get<Mesh>(L"Terrain");

	_material->SetInt(1, _sizeX);
	_material->SetInt(2, _sizeZ);
	_material->SetFloat(0, _maxTesselation);

	Ref<Texture> heightMap = GET_SINGLE(Resources)->Get<Texture>(L"HeightMap_0");
	Vec2 vec = Vec2(heightMap->GetWidth(), heightMap->GetHeight());

	_material->SetVec2(0, Vec2(heightMap->GetWidth(), heightMap->GetHeight()));
	_material->SetVec2(1, Vec2(1000.0f, 5000.0f));
	_material->SetTexture(2, heightMap);

	Ref<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	meshRenderer->mesh = (GET_SINGLE(Resources)->LoadTerrainMesh(sizeX, sizeZ));
	meshRenderer->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Terrain"));
	
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
	_material->SetVec4(0, Vec4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f));

}

void Terrain::EditorUpdate()
{
	string guiName = "";

	if (ImGui::CollapsingHeader("Terrain")) {
		if (ImGui::CollapsingHeader("Property")) {

		}

		if (ImGui::CollapsingHeader("Base")) {
			if (_material->GetTexture(0) == nullptr) {
				ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(_material->GetTexture(0), ImVec2(50.0f, 50.0f));
				guiName = Utils::Wstr2Str(_material->GetTexture(0)->name);
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					if (EDITOR->PickObject->name == L"Default") {
						_material->SetTexture(0, nullptr);
					}
					else {
						_material->SetTexture(0, static_pointer_cast<Texture>(EDITOR->PickObject));
					}
					EDITOR->PickObject = nullptr;
				}
				ImGui::EndDragDropTarget();
			}


		}
		if (ImGui::CollapsingHeader("Normal")) {
			if (_material->GetTexture(1) == nullptr) {
				ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(_material->GetTexture(1), ImVec2(50.0f, 50.0f));
				guiName = Utils::Wstr2Str(_material->GetTexture(1)->name);
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					if (EDITOR->PickObject->name == L"Default") {
						_material->SetTexture(1, nullptr);
					}
					else {
						_material->SetTexture(1, static_pointer_cast<Texture>(EDITOR->PickObject));
					}
					EDITOR->PickObject = nullptr;
				}
				ImGui::EndDragDropTarget();
			}


		}
		if (ImGui::CollapsingHeader("HeightMap")) {
			if (_material->GetTexture(2) == nullptr) {
				ImGui::Image(EDITOR->DefaultImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(_material->GetTexture(2), ImVec2(50.0f, 50.0f));
				guiName = Utils::Wstr2Str(_material->GetTexture(2)->name);
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					if (EDITOR->PickObject->name == L"Default") {
						_material->SetTexture(2, nullptr);
					}
					else {
						_material->SetTexture(2, static_pointer_cast<Texture>(EDITOR->PickObject));
					}
					EDITOR->PickObject = nullptr;
				}
				ImGui::EndDragDropTarget();
			}
		}

	}
}
