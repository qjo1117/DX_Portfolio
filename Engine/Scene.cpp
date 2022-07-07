#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "Light.h"
#include "Resources.h"

#include "MeshRenderer.h"
#include "Material.h"
#include "Shader.h"
#include "ParticleSystem.h"
#include "Transform.h"

Scene::Scene() : 
	_light(_gameObjects[static_cast<uint8>(LAYER_TYPE::LIGHT)]),
	_camera(_gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)])
{
	_objects.reserve(1000 * static_cast<uint8>(LAYER_TYPE::END));
	for (uint32 i = 0; i < static_cast<uint8>(LAYER_TYPE::END); ++i) {
		_gameObjects[i].reserve(1000);
	}
}

Scene::~Scene()
{
	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (vector<Ref<GameObject>>& gameObjects : _gameObjects) {
		for (Ref<GameObject>& obj : gameObjects) {
			obj = nullptr;
		}
	}
	for (Ref<GameObject>& obj : _objects) {
		obj = nullptr;
	}
}

void Scene::Awake()
{
	for (const Ref<GameObject>& object : _objects) {
		if (object == nullptr) {		// 이건 바로 삭제하지않고 FinalUpdate부분에서 삭제 요청 처리를
										// 처리하는 부분이 추가되면 nullptr문제가 없을테니 빼자
			continue;
		}
		if (object->isActive == false) {
			continue;
		}

		object->Awake();
	}
}

void Scene::Start()
{
	//for (const Ref<GameObject>& object : _objects) {
	//	if (object == nullptr) {
	//		continue;
	//	}
	//	if (object->GetActive() == false) {
	//		continue;
	//	}

	//	object->Start();
	//}

	for (const Ref<GameObject>& object : _parentBaseObjects) {
		if (object == nullptr) {
			continue;
		}
		object->Start();
	}
}

void Scene::Update()
{
	/* ----- Layer방식 ----- */
	//for (uint32 i = 0; i < _layerSize; ++i) {
	//	if (IsCulled(i)) {
	//		continue;
	//	}
	//	const vector<Ref<GameObject>>& vecObject = _gameObjects[i];
	//	for (const Ref<GameObject>& go : vecObject) {
	//		if (go == nullptr) {
	//			continue;
	//		}
	//		if (go->GetActive() == false) {
	//			continue;
	//		}
	//		go->Update();
	//	}
	//}

	SortParantGameObject();


	for (const Ref<GameObject>& object : _parentBaseObjects) {
		if (object == nullptr) {
			continue;
		}

		object->Update();
	}
}

void Scene::LateUpdate()
{
	for (const Ref<GameObject>& object : _parentBaseObjects) {
		if (object == nullptr) {
			continue;
		}

		object->LateUpdate();
	}
}

void Scene::FinalUpdate()
{
	//for (const Ref<GameObject>& object : _objects) {
	//	if (object == nullptr) {
	//		continue;
	//	}
	//	if (object->GetActive() == false) {
	//		continue;
	//	}

	//	object->FinalUpdate();
	//}

	for (const Ref<GameObject>& object : _parentBaseObjects) {
		if (object == nullptr) {
			continue;
		}

		object->FinalUpdate();
	}
}

void Scene::ClearRTV()
{

	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::PICK)->ClearRenderTargetView();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->ClearRenderTargetView();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

}

void Scene::SortSceneObjects()
{
	ClearRenderList();

	uint32 size = _objects.size();
	uint32 activeSize = size;
	for (uint32 i = 0; i < size; ++i) {
		Ref<GameObject>& obj = _objects[i];


		if (IsCulled(obj->GetLayer())) {
			continue;
		}

		if (obj->isActive == false) {
			_vecNonActived.push_back(obj);
			continue;
		}
		else {
			_vecActived.push_back(obj);
		}

		/* ----- Shadow ----- */
		if (obj->isShadow == false) {
			_vecShadow.push_back(obj);
		}

		if (obj->GetMeshRenderer() == nullptr && obj->GetParticleSystem() == nullptr) {
			continue;
		}

		if (obj->GetMeshRenderer()->GetMaterial()) {
			SHADER_TYPE type = obj->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
			switch (type) {
			case SHADER_TYPE::FORWARD:
				_vecForward.push_back(obj);
				break;
			case SHADER_TYPE::DEFERRED:
				_vecDeferred.push_back(obj);
				break;
			}
		}
		_vecRender.push_back(obj);
	}

}

void Scene::ClearRenderList()
{
	_vecDeferred.clear();
	_vecForward.clear();
	_vecParticle.clear();
	_vecShadow.clear();
	_vecRender.clear();

	_vecActived.clear();
	_vecNonActived.clear();

	int32 size = _objects.size();

	if (size < _vecRender.size()) {
		_vecShadow.reserve(size);
		_vecRender.reserve(size);

		_vecActived.reserve(size);
		_vecNonActived.reserve(size);
	}
}

void Scene::RenderShadow()
{
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->OMSetRenderTargets();

	for (const Ref<GameObject> light : _gameObjects[static_cast<uint8>(LAYER_TYPE::LIGHT)]) {
		if (light->isActive == false) {
			continue;
		}
		if (light->GetLight()->GetLightType() != LIGHT_TYPE::DIRECTIONAL_LIGHT) {
			continue;
		}
		
		light->GetLight()->RenderShadow();
	}

	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->WaitTargetToResource();
}

void Scene::RenderPick()
{
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::PICK)->OMSetRenderTargets();

	_gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)][0]->GetCamera()->Render_Pick();

	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::PICK)->WaitTargetToResource();
}

void Scene::RenderDeferred()
{
	// MainCamera가 렌더링을 해준다.
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	// 0번을 메인카메라라고 지정을 하고 나머지 Deferred에 필요한 RTV를 준비한다.
	Ref<Camera> mainCamera = _gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)][0]->GetCamera();

	mainCamera->SortGameObject();
	mainCamera->Render_Deferred();

	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();
}

void Scene::Render()
{
	PushLightData();

	ClearRTV();

	SortSceneObjects();

	RenderShadow();

	RenderDeferred();

	// TODO : 나중에 다시 수정하도록 하자. Picking
	//RenderPick();

	RenderLights();
	RenderFinal();

	GET_SINGLE(ColliderManager)->Render();

	RenderFoward();


}

void Scene::RenderLights()
{
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	// 광원을 그린다.
	for (const Ref<GameObject>& light : _gameObjects[static_cast<uint8>(LAYER_TYPE::LIGHT)]) {
		light->GetLight()->Render();
	}

	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();

}

void Scene::RenderFinal()
{
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	Ref<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Final");
	material->PushGraphicsData();
	GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle")->Render();
}

void Scene::RenderFoward()
{
	Ref<Camera> mainCamera = _gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)][0]->GetCamera();
	mainCamera->Render_Forward();

	// UI, Forward를 그려준다.
	for (const auto& camera : _gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)]) {
		if (camera->GetCamera() == nullptr) {
			continue;
		}
		if (camera->isActive == false) {
			continue;
		}
		if (camera->GetCamera() == mainCamera) {
			continue;
		}

		camera->GetCamera()->SortForwadObject();
		camera->GetCamera()->Render_Forward();
	}
}

Ref<class Camera> Scene::GetMainCamera()
{
	return _gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)][0]->GetCamera();
}

void Scene::PushLightData()
{
	LightParams lightParams = {};

	for (const Ref<GameObject>& light : _gameObjects[static_cast<uint8>(LAYER_TYPE::LIGHT)]) {
		if (light->GetLight() == nullptr) {
			continue;
		}
		if (light->isActive == false) {
			continue;
		}

		const LightInfo& lightInfo = light->GetLight()->GetLightInfo();

		light->GetLight()->SetLightIndex(lightParams.lightCount);		// 몇번째 빛인지 알려준다.

		lightParams.lights[lightParams.lightCount] = lightInfo;
		lightParams.lightCount += 1;
	}

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::GLOBAL)->SetGraphicsGlobalData(&lightParams, sizeof(lightParams));
}

void Scene::AddGameObject(Ref<GameObject> gameObject, uint32 layer)
{
	/* ----- 주의사항 ----- */
	// Camera, Light는 따로 순회해서 처리해야하는 작업이 있으므로 따로 빼서 LAYER에 강제로 넣어준다.

	if (gameObject->GetLight()) {
		layer = static_cast<uint8>(LAYER_TYPE::LIGHT);
	}
	else if (gameObject->GetCamera()) {
		layer = static_cast<uint8>(LAYER_TYPE::CAMERA);
	}

	// 만약 사용자가 따로 레이어를 설정한 경우에는 그에 맞게 넣어준다.
	if (gameObject->GetLayer() != static_cast<uint8>(LAYER_TYPE::DEFAULT)) {
		layer = gameObject->GetLayer();
	}

	_gameObjects[layer].push_back(gameObject);
	_objects.push_back(gameObject);
	gameObject->SetLayer(layer);
}

void Scene::SortParantGameObject()
{
	_parentBaseObjects.clear();

	uint32 size = _objects.size();
	for (uint32 i = 0; i < size; ++i) {
		if (_objects[i]->GetTransform()->GetParent().lock() == nullptr) {
			_parentBaseObjects.push_back(_objects[i]);
		}
	}

}

void Scene::AddGameObject(Ref<GameObject> gameObject, LAYER_TYPE layer)
{
	AddGameObject(gameObject, static_cast<uint32>(layer));
}

void Scene::RemoveObject(Ref<GameObject> gameObject, LAYER_TYPE layer)
{
	if (layer != LAYER_TYPE::END) {
		/* ----- Layer를 지정해준 경우 Layer하나만 순회를 해본다. ----- */
		RemoveObject(gameObject, static_cast<uint32>(layer));
	}
	else {
		/* ----- Layer를 안 지정해준 경우 전체를 순회한다. ----- */
		for (uint32 i = 0; i < static_cast<uint32>(LAYER_TYPE::END); ++i) {
			if (RemoveObject(gameObject, i) == true) {
				// 찾았으면 빠르게 순회를 끊는다.
				break;
			}
		}
	}

	auto findGo = std::find(_objects.begin(), _objects.end(), gameObject);
	_objects.erase(findGo);
}

bool Scene::RemoveObject(Ref<GameObject> gameObject, uint32 layer)
{
	/* ----- Layer를 지정해준 경우 Find를 통해서 찾고 삭제한다. ----- */
	auto findGo = std::find(_gameObjects[layer].begin(), _gameObjects[layer].end(), gameObject);
	if (findGo != _gameObjects[layer].end()) {
		_gameObjects[layer].erase(findGo);
		return true;
	}
	return false;
}
