#include "pch.h"
#include "Utils.h"
#include "ToolScene.h"
#include "RenderTargetGroup.h"
#include "GameObject.h"
#include "Engine.h"
#include "Resources.h"
#include "PathManager.h"

#include "Material.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Timer.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "MeshData.h"
#include "ParticleSystem.h"
#include "Component.h"
#include "Rigidbody.h"

#include "CameraController.h"
#include "RotationXComponent.h"
#include "PlayerController.h"

#include "Billboard.h"
#include "DragonController.h"

#include "ColliderManager.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshCollider.h"

#include "TestEditor.h"
#include "HierarchyEditor.h"

ToolScene::ToolScene()
{
	_strName.reserve(MAX_PATH);

#pragma region SkyBox
	{
		Ref<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->name = (L"SkyBox");
		Ref<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			meshRenderer->mesh = (GET_SINGLE(Resources)->LoadSphereMesh());
		}
		{
			Ref<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Skybox");
			Ref<Texture> texture = GET_SINGLE(Resources)->Get<Texture>(L"Skybox");
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		skybox->isFrustum = false;
		AddGameObject(skybox, LAYER_TYPE::SKYBOX);
	}
#pragma endregion

#pragma region Main Camera
	{
		Ref<GameObject> go = make_shared<GameObject>();
		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<Camera>());
		go->AddComponent(make_shared<CameraController>());
		go->name = (L"MainCamera");
		_camera = go;
		_camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.f, 0.0f));
		AddGameObject(_camera, LAYER_TYPE::CAMERA);

		_camera->GetCamera()->SetCullingMaskLayerOnOff(LAYER_TYPE::UI, true); // UI는 안찍음
	}
#pragma endregion

#pragma region UI Camera
	{
		Ref<GameObject> go = make_shared<GameObject>();
		go = go;

		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<Camera>());

		go->name = (L"UI_Camera");

		go->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		go->GetCamera()->CameraType = (PROJECTION_TYPE::ORTHOGRAPHIC);

		go->GetCamera()->SetCullingMaskAll();
		go->GetCamera()->SetCullingMaskLayerOnOff(LAYER_TYPE::UI, false);	// UI만 찍음

		AddGameObject(go, LAYER_TYPE::CAMERA);
	}
#pragma endregion

#pragma region ComputeShader
	{
		Ref<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		// UAV 용 Texture 생성
		Ref<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		Ref<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVCpuHandle(), UAV_REGISTER::u0);

		// 쓰레드 그룹 (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
#pragma endregion


#pragma region UI_Test
	{
		Ref<GameObject> uiTest = make_shared<GameObject>();
		uiTest->AddComponent(make_shared<Transform>());
		uiTest->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		uiTest->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
		uiTest->name = (L"RenderTargetView");

		for (int32 i = 0; i < 6; ++i) {
			Ref<GameObject> renderTarget = make_shared<GameObject>();
			renderTarget->name = (L"RenderTargetView" + std::to_wstring(i));
			renderTarget->AddComponent(make_shared<Transform>());
			renderTarget->GetTransform()->SetLocalPosition(Vec3(-350.0f + (i * 100.0f), 350.0f, 500.0f));
			renderTarget->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

			Ref<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			meshRenderer->mesh = (GET_SINGLE(Resources)->LoadRectangleMesh());

			{
				Ref<Texture> texture;
				if (i < 3) {
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
				}
				else if (i < 5) {
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
				}
				else if (i < 6) {
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
				}
				else if (i < 7) {
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::PICK)->GetRTTexture(0);
				}
				else {
					texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");
				}

				Ref<Material> material = make_shared<Material>();
				material->SetTexture(0, texture);
				material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Texture"));
				meshRenderer->SetMaterial(material);
			}

			renderTarget->GetTransform()->SetParent(uiTest->GetTransform());
			renderTarget->AddComponent(meshRenderer);
			_renderTargetView.push_back(renderTarget);
			AddGameObject(renderTarget, LAYER_TYPE::UI);
		}

		AddGameObject(uiTest, LAYER_TYPE::DEFAULT);
	}
#pragma endregion

#pragma region Directional Light
	{
		Ref<GameObject> light = make_shared<GameObject>();
		light->name = (L"Directional Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 1000.0f, 0.0f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.0f, -1.0f, 0.0f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.7f, 0.7f, 0.7f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));

		AddGameObject(light);
	}
#pragma endregion

#pragma region Point Light
	//{
	//	Ref<GameObject> light = make_shared<GameObject>();
	//	light->SetName(L"Point Light");
	//	light->AddComponent(make_shared<Transform>());
	//	light->GetTransform()->SetLocalPosition(Vec3(0.f, 50.f, -50.f));
	//	light->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
	//	light->AddComponent(make_shared<Light>());
	//	light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
	//	light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
	//	light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
	//	light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
	//	light->GetLight()->SetLightRange(200.0f);

	//	AddGameObject(light);
	//}
#pragma endregion

#pragma region Particle
	//{
	//	Ref<GameObject> particle = make_shared<GameObject>();
	//	particle->SetName(L"Particle");

	//	particle->AddComponent(make_shared<Transform>());
	//	particle->AddComponent(make_shared<ParticleSystem>());
	//	particle->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 100.0f));
	//	particle->GetParticleSystem()->SetTexture(GET_SINGLE(Resources)->Load<Texture>(L"Butterfly", TEXTURE_PATH + L"Particle\\Butterfly.png"));
	//	
	//	AddGameObject(particle);
	//}
#pragma endregion


#pragma region Collider_Test
	{
		//for (int32 i = 0; i < 50; ++i) {
		//	Ref<GameObject> box = make_shared<GameObject>();
		//	box->name = L"Box" + to_wstring(i);
		//	box->AddComponent(make_shared<Transform>());
		//	box->AddComponent(make_shared<MeshRenderer>());

		//	box->AddComponent(make_shared<BoxCollider>());
		//	GET_SINGLE(ColliderManager)->AddCollider(box->GetCollider());

		//	box->GetTransform()->localPosition = (-Vec3::Right * 150.0f * (i % 10)) + (Vec3::Up * 150.0f * (i / 10.0f));
		//	box->GetTransform()->localScale = Vec3::One * 100.0f;

		//	box->GetMeshRenderer()->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Defualt"));
		//	box->GetMeshRenderer()->mesh = GET_SINGLE(Resources)->LoadCubeMesh();

		//	AddGameObject(box, LAYER_TYPE::DEFAULT);
		//}
	}
#pragma endregion

#pragma region Plane
	{
		Ref<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<MeshRenderer>());
		obj->AddComponent(make_shared<Terrain>());
		obj->AddComponent(make_shared<MeshCollider>());

		obj->GetTerrain()->Init(30, 30);
		//obj->AddComponent(make_shared<Terrain>());
		//obj->GetTerrain()->Init(15, 15);

		obj->name = L"Ground";
		obj->isFrustum = false;

		obj->GetTransform()->localPosition = (-Vec3::Up * 150.0f) - Vec3::Right * 700.0f + Vec3::Forward * 700.0f;
		obj->GetTransform()->localScale = Vec3::One * 50.0f;

		AddGameObject(obj, LAYER_TYPE::GROUND);
	}
#pragma endregion


#pragma region RayCast_Test
	{
		Ref<GameObject> parent = make_shared<GameObject>();
		parent->AddComponent(make_shared<Transform>());
		for (int32 i = 0; i < 10; ++i) {
			Ref<GameObject> obj = make_shared<GameObject>();
			obj->AddComponent(make_shared<Transform>());
			obj->AddComponent(make_shared<MeshRenderer>());
			obj->AddComponent(make_shared<BoxCollider>());

			obj->GetTransform()->localPosition = ((-Vec3::Right * 150.0f) * (i % 10)) + Vec3::Right * 750.0f + Vec3::Up * 130.0f * (i / 10) - Vec3::Forward * 100.0f;
			obj->GetTransform()->localScale = Vec3::One * 100.0f;

			obj->GetMeshRenderer()->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Defualt"));
			obj->GetMeshRenderer()->mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			
			obj->GetTransform()->SetParent(parent->GetTransform());

			AddGameObject(obj, LAYER_TYPE::ENEMY);
		}

		AddGameObject(parent, LAYER_TYPE::ENEMY);
	}

	{
		Ref<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<MeshRenderer>());
		obj->AddComponent(make_shared<SphereCollider>());
		obj->AddComponent(make_shared<PlayerController>());
		obj->AddComponent(make_shared<Rigidbody>());

		obj->name = L"Player";
		obj->isShadow = false;

		obj->GetTransform()->localPosition = (Vec3::Right * 150.0f);
		obj->GetTransform()->localScale = Vec3::One * 100.0f;

		obj->layerType = static_cast<uint32>(LAYER_TYPE::PLAYER);

		obj->GetMeshRenderer()->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Player"));
		obj->GetMeshRenderer()->mesh = GET_SINGLE(Resources)->LoadSphereMesh();

		AddGameObject(obj, LAYER_TYPE::PLAYER);
	}
#pragma endregion

#pragma region RayCastHit Test
	{
		Ref<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<MeshRenderer>());
		obj->AddComponent(make_shared<SphereCollider>());

		obj->name = L"PickSphere";

		obj->GetTransform()->localScale = Vec3::One * 10.0f;

		obj->GetMeshRenderer()->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Player"));
		obj->GetMeshRenderer()->mesh = GET_SINGLE(Resources)->LoadSphereMesh();

		AddGameObject(obj, LAYER_TYPE::DEFAULT);

		_pickSphere = obj;
	}
#pragma endregion

#pragma region Animation
	//{
	//	Ref<GameObject> meshData = GET_SINGLE(Resources)->Get<GameObject>(L"OrangeBot");
	//	meshData->AddComponent(make_shared<DragonController>());
	//	meshData->AddComponent(make_shared<BoxCollider>());
	//	AddGameObject(meshData);

	//}
#pragma endregion



	GEngine->GetGraphicsCmdQueue()->WaitSync();

	_defauleImage = GET_SINGLE(Resources)->Get<Texture>(L"Default");
	_pickTexture = GET_SINGLE(Resources)->Get<Texture>(L"Star");
}

ToolScene::~ToolScene()
{
	_camera = nullptr;
	_pickSphere = nullptr;
}

void ToolScene::Awake()
{
	Scene::Awake();
}

void ToolScene::Start()
{
	Scene::Start();
}

void ToolScene::Update()
{
	Scene::Update();

	// Pick
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON) == true) {
		if (ImGui::IsAnyItemActive()) {
			return;
		}
		Ref<GameObject> picked = GET_SINGLE(SceneManager)->Pick(INPUT->GetMousePos().x, INPUT->GetMousePos().y);
		if (picked != nullptr) {
			HierarchyEditor::GetI()->PickObject = picked;
		}

	}
}

void ToolScene::LateUpdate()
{
	Scene::LateUpdate();
}

void ToolScene::FinalUpdate()
{
	Scene::FinalUpdate();
}
