#include "pch.h"
#include "Utils.h"
#include "ToolScene.h"
#include "RenderTargetGroup.h"
#include "GameObject.h"
#include "Engine.h"
#include "EngineGUI.h"
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
#include "SphereCollider.h"
#include "ParticleSystem.h"
#include "Component.h"

#include "CameraController.h"
#include "RotationXComponent.h"
#include "PlayerController.h"

#include "Billboard.h"
#include "DragonController.h"


ToolScene::ToolScene()
{
	_strName.reserve(MAX_PATH);

#pragma region SkyBox
	{
		Ref<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetName(L"SkyBox");
		Ref<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadSphereMesh());
		}
		{
			Ref<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Skybox");
			Ref<Texture> texture = GET_SINGLE(Resources)->Get<Texture>(L"Skybox");
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		skybox->SetCheckFrustum(false);
		AddGameObject(skybox, LAYER_TYPE::SKYBOX);
	}
#pragma endregion

	// X
#pragma region FBX
	//{
	//	Ref<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"Dragon.fbx");
	//	vector<Ref<GameObject>> gameObjects = meshData->Instantiate();
	//	
	//	Ref<GameObject> parent = make_shared<GameObject>();
	//	parent->AddComponent(make_shared<Transform>());

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetName(L"Dragon");
	//		gameObject->SetCheckFrustum(false);
	//
	//		gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 300.f));
	//		gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	//		gameObject->GetTransform()->SetParent(parent->GetTransform());
	//		gameObject->AddComponent(make_shared<DragonController>());
	//		AddGameObject(gameObject);
	//	}
	//	AddGameObject(parent);
	//}
#pragma endregion

	// X
#pragma region PlayerParent
	//{
	//	Ref<GameObject> parent = make_shared<GameObject>();
	//	parent->AddComponent(make_shared<Transform>());
	//	AddGameObject(parent);
	//	_player = parent;
	//}
#pragma endregion

	// X
#pragma region Player
	//{
	//	for (uint32 i = 0; i < 100; ++i) {
	//		Ref<GameObject> go = make_shared<GameObject>();
	//		go->Init();
	//		go->SetName(L"Player" + std::to_wstring(i));
	//		go->AddComponent(make_shared<Transform>());
	//		go->AddComponent(make_shared<SphereCollider>());

	//		go->SetShadow(i % 2 == 1 ? false : true);
	//		Ref<MeshRenderer> renderer = make_shared<MeshRenderer>();
	//		renderer->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Player"));
	//		renderer->SetMesh(GET_SINGLE(Resources)->LoadCylinderMesh());
	//		go->AddComponent(renderer);

	//		go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
	//		go->GetTransform()->SetLocalPosition(Vec3(-130.0f + 130 * (i % 10), 100.0f, -130.0f + 130 * (i / 10) ));
	//		
	//		dynamic_pointer_cast<SphereCollider>(go->GetCollider())->SetRadius(0.5f);
	//		dynamic_pointer_cast<SphereCollider>(go->GetCollider())->SetCenter(Vec3(0.0f, 0.0f, 0.0f));
	//		
	//		AddGameObject(go, LAYER_TYPE::DEFAULT);
	//		go->GetTransform()->SetParent(_player->GetTransform());
	//	}
	//}
#pragma endregion

#pragma region Tessellation
	{
		Ref<GameObject> go = make_shared<GameObject>();
		go->SetName(L"Tessellation");
		go->AddComponent(make_shared<Transform>());
		go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		go->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));

		Ref<MeshRenderer> renderer = make_shared<MeshRenderer>();
		renderer->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Tessellation"));
		renderer->SetMesh(GET_SINGLE(Resources)->LoadRectangleMesh());
		go->AddComponent(renderer);
		
		_tessellation = go;

		go->AddComponent(make_shared<Billboard>());

		AddGameObject(go, LAYER_TYPE::DEFAULT);
	}
#pragma endregion

#pragma region Terrain
	{
		Ref<GameObject> go = make_shared<GameObject>();
		go->AddComponent(make_shared<Transform>());
		go->GetTransform()->SetLocalPosition(Vec3(-100.0f, -200.0f, 300.0f));
		go->GetTransform()->SetLocalScale(Vec3(50.0f, 200.0f, 50.0f));
		go->SetName("Terrain");
		go->SetShadow(true);

		go->AddComponent(make_shared<Terrain>());
		go->GetTerrain()->Init(64, 64);

		AddGameObject(go, LAYER_TYPE::DEFAULT);
	} 
#pragma endregion

#pragma region Main Camera
	{
		Ref<GameObject> go = make_shared<GameObject>();
		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<Camera>());
		go->AddComponent(make_shared<CameraController>());
		go->SetName(L"MainCamera");
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

		go->SetName(L"UI_Camera");

		go->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		go->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);

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
		uiTest->SetName(L"RenderTarget");

		for (int32 i = 0; i < 8; ++i) {
			Ref<GameObject> renderTarget = make_shared<GameObject>();
			renderTarget->SetName("RenderTarget" + std::to_string(i));
			renderTarget->AddComponent(make_shared<Transform>());
			renderTarget->GetTransform()->SetLocalPosition(Vec3(-350.0f + (i * 100.0f), 350.0f, 500.0f));
			renderTarget->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

			Ref<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadRectangleMesh());

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
		light->SetName(L"Directional Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 3000.0f, 0.0f));
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

	


	GEngine->GetGraphicsCmdQueue()->WaitSync();

	/* ----- 바인딩 부분 ----- */
	GUI->AddFunction([=]() { TestEditor(); });
	GUI->AddFunction([=]() { TestObjects(); });
	GUI->AddFunction([=]() { TestButtonCreateObject(); });
	GUI->AddFunction([=]() { TestResources(); });
	GUI->AddFunction([=]() { TestRenderTargetView(); });
	GUI->AddFunction([=]() { TestEditorWindow(); });

	_defauleImage = GET_SINGLE(Resources)->Get<Texture>(L"Default");
	_pickTexture = GET_SINGLE(Resources)->Get<Texture>(L"Star");

	_pickTestDraw.resize(1280 * 640);
}

ToolScene::~ToolScene()
{
	_goPick = nullptr;
	_player = nullptr;
	_camera = nullptr;
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

	/* ----- TODO Test SceneChange ----- */
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE)) {
		GET_SINGLE(SceneManager)->LoadScene(SCENE_TYPE::START);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::DEL)) {
		RemoveSelectObject();
	}

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON)) {

		ComPtr<ID3D12Resource> tex = _defauleImage->GetTex2D();
		//ComPtr<ID3D12Resource> pWritableResource = tex->Alloc(dwWidth, dwHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
		D3D12_RESOURCE_DESC desc = tex->GetDesc();

		
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

void ToolScene::Create()
{
	for (auto& func : _createList) {
		func();
	}
	_createList.clear();
}

void ToolScene::TestEditor()
{
	/* ----- 객체의 정보를 보여줍니다. ----- */
	ImGui::Begin("Editor");

	// 선택한 녀석이 없으면 종료
	if (_goPick == nullptr) {
		ImGui::End();
		return;
	}

	// Name And Layer
	{
		static char strTemp[50] = {};
		::sprintf_s(strTemp, "%s", _goPick->GetGUIName().data());

		bool active = _goPick->GetActive();
		ImGui::Checkbox("##Check", &active);
		_goPick->SetActive(active);
		ImGui::SameLine();

		::strcpy_s(_text, _goPick->GetGUIName().data());
		ImGui::InputText("##Nmae", _text, MAX_PATH);
		_goPick->SetName(_text);
		ImGui::SameLine();

		string layer = EnumToStr((LAYER_TYPE)_goPick->GetLayer());				// 매크로 사용
		::sprintf_s(strTemp, 50, "/ Layer : %s", layer.data());
		ImGui::Text(strTemp);
	}

	ImGuiIO& io = ImGui::GetIO();

	// Transform.
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::TRANSFORM) != nullptr) {
		TransformComponent();
	}
	
	// MeshRenderer
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER) != nullptr) {
		MeshRendererComponent();
	}

	// Camera
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::CAMERA) != nullptr) {
		CameraComponent();
	}

	// Light
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::LIGHT) != nullptr) {
		LightComponent();
	}	
	
	// Particle
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::PARTICLE_SYSTEM) != nullptr) {
		ParticleComponent();
	}

	// Terrain
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::TERRAIN) != nullptr) {
		TerrainComponent();
	}
	


	// Constorm Scripts
	for (auto& script : _goPick->GetScripts()) {
		if(ImGui::CollapsingHeader(script.first.data())) {
			script.second->DrawGUI();
		}
	}

	if (ImGui::Button("AddComponent", ImVec2(100.0f, 35.0f)))
	{
		ImGui::OpenPopup("SelectComponent");
		ImGui::Text("Edit name:");
	}

	if (ImGui::BeginPopup("SelectComponent"))
	{
		ImGui::Text("Components");
		ImGui::Separator();
		string guiName;

		auto& components = GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::COMPONENT);
		for (auto& component : components) {
			if (ImGui::Selectable(component.second->GetGUIName().data())) {
				_goPick->AddComponent(static_pointer_cast<Component>(component.second));
			}
		}

		::memset(_text, 0, MAX_PATH);
		ImGui::InputText("##edit", _text, IM_ARRAYSIZE(_text));
		ImGui::EndPopup();
	}

	ImGui::End();
}

void ToolScene::TestObjects()
{
	/* ----- 객체들이 뭐뭐 있는지 보여줍니다. ----- */
	ImGui::Begin("ObjectList");
	ImGui::SetWindowSize("ObjectList", ImVec2(200, 600));

	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::TreeNode("Tool Scene Object List")) {
		uint32 count = 0;

		// Editor Object Index Change
		vector<Ref<GameObject>>& objects = GetAllGameObjects();
		for (int32 i = 0; i < objects.size(); ++i) {
			if (objects[i]->GetTransform()->GetParent().lock() != nullptr) {
				continue;
			}
			ParentTree(objects[i]);
		}

		ImGui::TreePop();
	}

	{
		static char fpsStr[30] = {};
		::sprintf_s(fpsStr, 30, "Fps : %d", GET_SINGLE(Timer)->GetFps());
		ImGui::Text(fpsStr);
	}
	{
		if (_dragPick) {
			ImGui::Text(_dragPick->GetGUIName().data());
		}
	}
	{
		if (_objPick) {
			ImGui::Text(_objPick->GetGUIName().data());
		}
	}

	ImGui::End();
}

void ToolScene::TestResources()
{
	ImGui::Begin("Resources");

	string guiName;

	if (ImGui::TreeNode("Texture")) {
		for (const pair<wstring, Ref<Object>>& tex : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::TEXTURE)) {
			guiName.assign(tex.first.begin(), tex.first.end());

			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("TEXTURE_DRAG", nullptr, 0);
				_objPick = tex.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Shader")) {
		for (const pair<wstring, Ref<Object>>& shader : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::SHADER)) {
			guiName.assign(shader.first.begin(), shader.first.end());
			
			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("SHADER_DRAG", nullptr, 0);
				_objPick = shader.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Material")) {
		for (const pair<wstring, Ref<Object>>& material : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::MATERIAL)) {
			guiName.assign(material.first.begin(), material.first.end());

			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("MATERIAL_DRAG", nullptr, 0);
				_objPick = material.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GameObject")) {
		for (const pair<wstring, Ref<Object>>& go : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::GAMEOBJECT)) {
			guiName.assign(go.first.begin(), go.first.end());
			
			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("GAMEOBJECT_DRAG", nullptr, 0);
				_objPick = go.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Compoenent")) {
		for (const pair<wstring, Ref<Object>>& component : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::COMPONENT)) {
			guiName.assign(component.first.begin(), component.first.end());
			
			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("COMPONENT_DRAG", nullptr, 0);
				_objPick = component.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Mesh")) {
		for (const pair<wstring, Ref<Object>>& mesh : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::MESH)) {
			guiName.assign(mesh.first.begin(), mesh.first.end());
			
			ImGui::Selectable(guiName.data());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("MESH_DRAG", nullptr, 0);
				_objPick = mesh.second;
				ImGui::EndDragDropSource();
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

void ToolScene::TestRenderTargetView()
{
	//ImGui::Begin("RenderTargetView");

	//Ref<RenderTargetGroup> renderTarget;

	//string guiName;

	//renderTarget = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER);
	//for (uint32 i = 0; i < RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT; ++i) {
	//	Ref<Texture> tex = renderTarget->GetRTTexture(i);
	//	ImGui::Image(tex, ImVec2(100.0f, 100.0f));
	//	guiName.assign(tex->GetName().begin(), tex->GetName().end());
	//	ImGui::Text(guiName.data());
	//}
	//renderTarget = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING);
	//for (uint32 i = 0; i < RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT; ++i) {
	//	Ref<Texture> tex = renderTarget->GetRTTexture(i);
	//	ImGui::Image(tex, ImVec2(100.0f, 100.0f));
	//	guiName.assign(tex->GetName().begin(), tex->GetName().end());
	//	ImGui::Text(guiName.data());
	//}

	//ImGui::End();

	ImGui::Begin("Tessellation Test");

	if (_tessellation != nullptr) {
		auto material = _tessellation->GetMeshRenderer()->GetMaterial();

		Vec4 vec = material->GetVec4(0);
		int iTemp[4] = { vec.x, vec.y, vec.z, vec.w };

		ImGui::InputInt(" 0",		&iTemp[0], 1);
		ImGui::InputInt(" 1",		&iTemp[1], 1);
		ImGui::InputInt(" 2",		&iTemp[2], 1);
		ImGui::InputInt("Inside",	&iTemp[3], 1);

		vec = { (float)iTemp[0], (float)iTemp[1] ,(float)iTemp[2] ,(float)iTemp[3] };

		material->SetVec4(0, vec);
	}

	ImGui::End();
}


void ToolScene::TestButtonCreateObject()
{
	ImGui::Begin("TestButton");

	if (ImGui::Button("CreateButton")) {		
		CreateCubeGameObject();
	}
	if (ImGui::Button("RemoveButton")) {
		RemoveSelectObject();
	}

	if (ImGui::Button("Save")) {
		SceneSerializered();
	}

	if (ImGui::Button("Load")) {
		DeSceneSerializered();
	}

	ImGui::End();
}

void ToolScene::TestEditorWindow()
{
	//ImGuiIO& io = ImGui::GetIO();
	//float viewManipulateRight = io.DisplaySize.x;
	//float viewManipulateTop = 0;

	//static ImGuiWindowFlags gizmoWindowFlags = 0;

	//ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
	//ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
	//ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
	//ImGuizmo::SetDrawlist();
	//float windowWidth = (float)ImGui::GetWindowWidth();
	//float windowHeight = (float)ImGui::GetWindowHeight();
	//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	//viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
	//viewManipulateTop = ImGui::GetWindowPos().y;
	//ImGuiWindow* window = ImGui::GetCurrentWindow();
	//gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

	//ImGui::End();
	//ImGui::PopStyleColor(1);
}

void ToolScene::TransformComponent()
{
	if (ImGui::CollapsingHeader("Transform")) {
		const float dragSpeed = 1.0f;
		Vec3 vecTemp;

		vecTemp = _goPick->GetTransform()->GetLocalPosition();
		ImGui::DragFloat3("Position", &vecTemp.x, dragSpeed, -10000.0f, 10000.0f);
		_goPick->GetTransform()->SetLocalPosition(vecTemp);

		vecTemp = _goPick->GetTransform()->GetLocalRotation();
		ImGui::DragFloat3("Rotation", &vecTemp.x, dragSpeed / 36, -10000.0f, 10000.0f);
		_goPick->GetTransform()->SetLocalRotation(vecTemp);


		vecTemp = _goPick->GetTransform()->GetLocalScale();
		ImGui::DragFloat3("Scale", &vecTemp.x, dragSpeed, -10000.0f, 10000.0f);
		_goPick->GetTransform()->SetLocalScale(vecTemp);
	}
}

void ToolScene::MeshRendererComponent()
{
	Ref<MeshRenderer> renderer = _goPick->GetMeshRenderer();
	Ref<Material> material = _goPick->GetMeshRenderer()->GetMaterial();
	string guiName;

	if (_goPick->GetTerrain()) {
		return;
	}

	if (ImGui::CollapsingHeader("MeshRenderer")) {
		ImGuiMesh(renderer);
		ImGuiMaterials(renderer);
		

		/* ----- 준비중 ----- */
		//if (ImGui::CollapsingHeader("Albedo")) {
		//	if (material->GetTexture(0) == nullptr) {
		//		ImGui::Image(_defauleImage, ImVec2(50.0f, 50.0f));
		//		guiName = "nullptr";
		//	}
		//	else {
		//		ImGui::Image(renderer->GetMaterial()->GetTexture(0), ImVec2(50.0f, 50.0f));
		//		guiName = material->GetTexture(0)->GetGUIName();
		//	}
		//	if (ImGui::BeginDragDropTarget()) {
		//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
		//			renderer->GetMaterial()->SetTexture(0, static_pointer_cast<Texture>(_objPick));
		//			_objPick = nullptr;
		//		}
		//		ImGui::EndDragDropTarget();
		//	}
		//	ImGui::SameLine();
		//	ImGui::Text(guiName.data());
		//}
		//if (ImGui::CollapsingHeader("Albedo")) {
		//	if (material->GetTexture(0) == nullptr) {
		//		ImGui::Image(_defauleImage, ImVec2(50.0f, 50.0f));
		//		guiName = "nullptr";
		//	}
		//	else {
		//		ImGui::Image(renderer->GetMaterial()->GetTexture(0), ImVec2(50.0f, 50.0f));
		//		guiName = material->GetTexture(0)->GetGUIName();
		//	}
		//	if (ImGui::BeginDragDropTarget()) {
		//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
		//			renderer->GetMaterial()->SetTexture(0, static_pointer_cast<Texture>(_objPick));
		//			_objPick = nullptr;
		//		}
		//		ImGui::EndDragDropTarget();
		//	}
		//	ImGui::SameLine();
		//	ImGui::Text(guiName.data());
		//}

	}




}

void ToolScene::CameraComponent()
{
	if (ImGui::CollapsingHeader("Camera")) {
		// TODO : 직교랑 절두랑 분기를 시켜줘야한다.
		float cameraNear = _goPick->GetCamera()->GetNear();
		float cameraFar = _goPick->GetCamera()->GetFar();
		float cameraFov = _goPick->GetCamera()->GetFov();

		ImGui::SliderFloat("Near", &cameraNear, 0.001f, cameraFar - 1);
		ImGui::SliderFloat("Far", &cameraFar, cameraNear + 1, 10000.0f);
		ImGui::SliderFloat("Fov", &cameraFov, 0.0f, XM_PI);

		_goPick->GetCamera()->SetNear(cameraNear);
		_goPick->GetCamera()->SetFar(cameraFar);
		_goPick->GetCamera()->SetFov(cameraFov);
	}
}

void ToolScene::LightComponent()
{
	if (ImGui::CollapsingHeader("Light")) {
		static float floatTemp[3] = {};
		Ref<Light> light = _goPick->GetLight();
		Vec4 vecTemp;

		/* ----- BaseLight ----- */
		if (ImGui::TreeNode("BaseLight")) {
			vecTemp = light->GetDiffuse();

			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::ColorEdit3("Diffuse", floatTemp);
			light->SetDiffuse(Vec3(floatTemp[0], floatTemp[1], floatTemp[2]));

			vecTemp = light->GetAmbient();
			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::ColorEdit3("Ambient", floatTemp);
			light->SetAmbient(Vec3(floatTemp[0], floatTemp[1], floatTemp[2]));

			vecTemp = light->GetSpecular();
			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::ColorEdit3("Specular", floatTemp);
			light->SetSpecular(Vec3(floatTemp[0], floatTemp[1], floatTemp[2]));

			ImGui::TreePop();
		}

		ImGui::Separator();

		/* ----- LightInfo ----- */
		if (ImGui::TreeNode("LightInfo")) {

			LightInfo info = _goPick->GetLight()->GetLightInfo();

			const float dragSpeed = (ImGui::GetIO().KeyShift) ? 0.5f : 0.05f;

			vecTemp = info.direction;
			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::DragFloat3("DirectionLight", floatTemp, dragSpeed, -2 * XM_PI, 2 * XM_PI);
			light->SetLightDirection(Vec3(floatTemp[0], floatTemp[1], floatTemp[2]));

			floatTemp[0] = 0.0f;
			if (info.lightType == static_cast<int32>(LIGHT_TYPE::DIRECTIONAL_LIGHT)) {

			}
			else if (info.lightType == static_cast<int32>(LIGHT_TYPE::POINT_LIGHT)) {
				floatTemp[0] = info.range;
				ImGui::DragFloat("Range", &floatTemp[0], dragSpeed, 0.0f, 10000.0f);
				light->SetLightRange(floatTemp[0]);

			}
			else if (info.lightType == static_cast<int32>(LIGHT_TYPE::SPOT_LIGHT)) {
				floatTemp[0] = info.angle;
				ImGui::DragFloat("Angle", &floatTemp[0], XM_PI / 360, 0.0f, XM_PI * 2);
				light->SetLightAngle(floatTemp[0]);

				floatTemp[1] = info.range;
				ImGui::DragFloat("Range", &floatTemp[1], dragSpeed, 0.0f, 10000.0f);
				light->SetLightRange(floatTemp[1]);
			}


			ImGui::TreePop();
		}
	}
}

void ToolScene::ParticleComponent()
{
	Ref<ParticleSystem> particle = _goPick->GetParticleSystem();
	if (ImGui::CollapsingHeader("ParticleSystem")) {
		float fTemp[4] = {};
		int iTemp[4] = {};

		iTemp[0] = particle->GetMaxParticleCount();
		ImGui::DragInt("ParticleCount", &iTemp[0], 1, 0, 10000);
		particle->SetMaxParticleCount(iTemp[0]);

		fTemp[0] = particle->GetMinLifeTime();
		fTemp[1] = particle->GetMaxLifeTime();
		ImGui::DragFloat2("ParticleLifeTime", fTemp, 0.1f, 0.0f, 1000.0f);
		particle->SetLifeTime(Vec2(fTemp[0], fTemp[1]));

		fTemp[0] = particle->GetMinSpeed();
		fTemp[1] = particle->GetMaxSpeed();
		ImGui::DragFloat2("ParticleSpeed", fTemp, 0.1f, 0.0f, 1000.0f);
		particle->SetSpeed(Vec2(fTemp[0], fTemp[1]));

		fTemp[0] = particle->GetStartScale();
		fTemp[1] = particle->GetEndScale();
		ImGui::DragFloat2("ParticleScale", fTemp, 0.1f, 0.0f, 1000.0f);
		particle->SetScale(Vec2(fTemp[0], fTemp[1]));

		Ref<Texture> tex = particle->GetTexture();
		ImGui::Image(tex, ImVec2(50.0f, 50.0f));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
				particle->SetTexture(static_pointer_cast<Texture>(_objPick));
				_objPick = nullptr;
			}
			ImGui::EndDragDropTarget();
		}
	}
}

void ToolScene::TerrainComponent()
{
	Ref<Material> material = _goPick->GetMeshRenderer()->GetMaterial();
	Ref<MeshRenderer> renderer = _goPick->GetMeshRenderer();
	string guiName;

	if (ImGui::CollapsingHeader("Terrain")) {
		if (ImGui::CollapsingHeader("Property")) {

		}

		if (ImGui::CollapsingHeader("Base")) {
			if (material->GetTexture(0) == nullptr) {
				ImGui::Image(_defauleImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(renderer->GetMaterial()->GetTexture(0), ImVec2(50.0f, 50.0f));
				guiName = material->GetTexture(0)->GetGUIName();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					renderer->GetMaterial()->SetTexture(0, static_pointer_cast<Texture>(_objPick));
					_objPick = nullptr;
				}
				ImGui::EndDragDropTarget();
			}


		}
		if (ImGui::CollapsingHeader("Normal")) {
			if (material->GetTexture(1) == nullptr) {
				ImGui::Image(_defauleImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(renderer->GetMaterial()->GetTexture(1), ImVec2(50.0f, 50.0f));
				guiName = material->GetTexture(1)->GetGUIName();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					renderer->GetMaterial()->SetTexture(1, static_pointer_cast<Texture>(_objPick));
					_objPick = nullptr;
				}
				ImGui::EndDragDropTarget();
			}


		}
		if (ImGui::CollapsingHeader("HeightMap")) {
			if (material->GetTexture(2) == nullptr) {
				ImGui::Image(_defauleImage, ImVec2(50.0f, 50.0f));
				guiName = "nullptr";
			}
			else {
				ImGui::Image(renderer->GetMaterial()->GetTexture(2), ImVec2(50.0f, 50.0f));
				guiName = material->GetTexture(2)->GetGUIName();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					renderer->GetMaterial()->SetTexture(2, static_pointer_cast<Texture>(_objPick));
					_objPick = nullptr;
				}
				ImGui::EndDragDropTarget();
			}


		}

	}
}

void ToolScene::ImGuiMaterials(Ref<MeshRenderer>& renderer)
{
	/* ----- Material ----- */
	string text;
	int32 size = renderer->GetMaterialSize();
	for (uint32 i = 0; i < size; ++i) {
		Ref<Material> material = renderer->GetMaterial(i);

		if (material == nullptr) {
			text = "nullptr";
		}
		else {
			text = material->GetGUIName();
		}
		
		ImGui::Separator();
		ImGui::Text("Material : %s", text.data());
		ImGui::BeginGroup();
		ImGui::EndGroup();
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MATERIAL_DRAG")) {
				renderer->SetMaterial(static_pointer_cast<Material>(_objPick));
				_objPick = nullptr;
			}
			ImGui::EndDragDropTarget();
		}

		/* ----- Shader ----- */
		ImGui::Text("Shader : %s", material->GetShader()->GetGUIName().data());
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SHADER_DRAG")) {
				renderer->GetMaterial()->SetShader(static_pointer_cast<Shader>(_objPick));
				_objPick = nullptr;
			}
			ImGui::EndDragDropTarget();
		}

		/* ----- Texture ----- */

		text = "Albedo##" + to_string(i);
		if (ImGui::CollapsingHeader(text.data())) {
			if (material->GetTexture(0) == nullptr) {
				ImGui::Image(_defauleImage, ImVec2(50.0f, 50.0f));
				text = "nullptr";
			}
			else {
				ImGui::Image(material->GetTexture(0), ImVec2(50.0f, 50.0f));
				text = material->GetTexture(0)->GetGUIName();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					material->SetTexture(0, static_pointer_cast<Texture>(_objPick));
					_objPick = nullptr;
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			Vec4 color = material->GetVec4(0);
			text = "Color##" + to_string(i);
			ImGui::ColorEdit4(text.data(), &color.x);
			material->SetVec4(0, color);
		}

		text = "Normal##" + to_string(i);
		if (ImGui::CollapsingHeader(text.data())) {
			if (material->GetTexture(1) == nullptr) {
				ImGui::Image(_defauleImage, ImVec2(50.0f, 50.0f));
				text = "nullptr";
			}
			else {
				ImGui::Image(material->GetTexture(1), ImVec2(50.0f, 50.0f));
				text = material->GetTexture(1)->GetGUIName();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					material->SetTexture(1, static_pointer_cast<Texture>(_objPick));
					_objPick = nullptr;
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text(text.data());
		}

		text = "Specular" + to_string(i);
		if (ImGui::CollapsingHeader(text.data())) {
			if (material->GetTexture(2) == nullptr) {
				ImGui::Image(_defauleImage, ImVec2(50.0f, 50.0f));
				text = "nullptr";
			}
			else {
				ImGui::Image(material->GetTexture(2), ImVec2(50.0f, 50.0f));
				text = material->GetTexture(2)->GetGUIName();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
					material->SetTexture(2, static_pointer_cast<Texture>(_objPick));
					_objPick = nullptr;
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::Text(text.data());
		}
	}


}

void ToolScene::ImGuiMesh(Ref<MeshRenderer>& renderer)
{
	bool isShadow = _goPick->IsShadow();
	if(ImGui::Button("Shadow")) {
		_goPick->SetShadow(!isShadow);
	}

	ImGui::Text("Mesh : ");
	ImGui::SameLine();
	ImGui::Text(renderer->GetMesh()->GetGUIName().data());
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_DRAG")) {
			renderer->SetMesh(static_pointer_cast<Mesh>(_objPick));
			_objPick = nullptr;
		}
		ImGui::EndDragDropTarget();
	}
}


void ToolScene::RemoveSelectObject()
{
	if (_goPick != nullptr) {
		RemoveObject(_goPick);
		_isPick = -1;
		_goPick = nullptr;
	}
}

void ToolScene::CreateCubeGameObject()
{
	Ref<GameObject> go = make_shared<GameObject>();
	Ref<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
	Ref<MeshRenderer> renderer = make_shared<MeshRenderer>();
	renderer->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Defualt"));
	renderer->SetMesh(mesh);

	go->AddComponent(make_shared<Transform>());
	go->AddComponent(renderer);

	go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
	go->GetTransform()->SetLocalPosition(Vec3(_random.Rand(-100.0f, 100.0f), _random.Rand(-100.0f, 100.0f), _random.Rand(-100.0f, 100.0f)));

	AddGameObject(go);
}

void ToolScene::SceneSerializered()
{
	// TODO : Stop Json으로 하자
	std::ofstream file;
	file.open("../Resources/Scene/ToolScene.txt");
	Json::StreamWriterBuilder builder;
	unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	Json::Value object;
	object["A_Count"] = GetAllGameObjects().size();

	for (Ref<GameObject>& go : GetAllGameObjects()) {
		go->Serializer(object);
	}

	writer->write(object, &file);

	file.close();
}

void ToolScene::DeSceneSerializered()
{


}

void ToolScene::ParentTree(const Ref<GameObject>& obj)
{
	uint32 node_flags = 0;
	_strName = obj->GetGUIName() + "##" + std::to_string(obj->GetID());

	if (obj->GetTransform()->GetChilds().size() == 0) {
		node_flags |= ImGuiTreeNodeFlags_Leaf; // ImGuiTreeNodeFlags_Bullet
		if (ImGui::TreeNodeEx(_strName.data(), node_flags)) {
			if (ImGui::IsItemActivated()) {
				_goPick = obj;
			}
			ImGui::TreePop();
		}
	}
	else {

		if (ImGui::TreeNodeEx(_strName.data(), node_flags)) {
			if (ImGui::IsItemActivated()) {
				_goPick = obj;
			}
			for (auto& child : obj->GetTransform()->GetChilds()) {
				ParentTree(child.lock()->GetGameObject());
			}

			ImGui::TreePop();
		}
	}
}
