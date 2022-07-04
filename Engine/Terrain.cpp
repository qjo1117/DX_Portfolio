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
	meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadTerrainMesh(sizeX, sizeZ));
	meshRenderer->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Terrain"));
	
	GetGameObject()->AddComponent(meshRenderer);
}

void Terrain::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::N_1)) {
		vector<Vertex>& vertices = _mesh->GetVertices();

		for (Vertex& vertex : vertices) {
			vertex.pos.y += 10.0f;
		}

		_mesh->Create(vertices, _mesh->GetIndices());
	}
}

void Terrain::FinalUpdate()
{
	Ref<Camera> mainCamera = GET_SINGLE(SceneManager)->GetCurrentScene()->GetMainCamera();
	if (mainCamera == nullptr) {
		return;
	}

	Vec3 cameraPos = mainCamera->GetTransform()->GetLocalPosition();
	_material->SetVec4(0, Vec4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f));

}
