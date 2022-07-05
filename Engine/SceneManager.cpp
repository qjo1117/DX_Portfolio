#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "GameObject.h"
#include "Engine.h"
#include "BaseCollider.h"

void SceneManager::Update()
{
	if (_currentScene == nullptr) {
		return;
	}
	_currentScene->Update();
	_currentScene->LateUpdate();
	_currentScene->FinalUpdate();

	Ref<Scene>& scene = _currentScene;

}

void SceneManager::Render()
{
	if (_currentScene) {
		_currentScene->Render();
	}

}

void SceneManager::LoadScene(SCENE_TYPE type)
{
	/* ----- Scene이 존재하는지 체크하고 있으면 씬전환을 한다. ----- */
	assert(_scenes[static_cast<uint32>(type)]);

	if (_currentScene == nullptr) {
		_currentScene = _scenes[static_cast<uint32>(type)];
	}
	else {
		// TODO : 내용물 초기화
		_currentScene = _scenes[static_cast<uint32>(type)];
	}
	_type = type;
	_currentScene->Awake();
	_currentScene->Start();
}

Ref<class GameObject> SceneManager::Pick(int32 screenX, int32 screenY)
{
	Ref<Camera> camera = GetCurrentScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix matProj = camera->matProjection;

	// 현재의 보완해야할 점
	// 1.	World Space에서의 변환으로 맞춰짐. 현재 Invert하는 Matrix와 Intersects에 있는 좌표를
	//		Local Space로 바꿔줘야 확인하는데 좋을 것 같음.
	//		이유 : MeshCollider같은 기능을 넣었을 경우 각 삼각형의 좌표가 float값을 넘어가는 경우가 생긴다.
	// 2.	ColliderManager를 만들어서 충돌체크 영역을 따로 만들어 준다.
	//		프레임별로 시간을 나눠주는 효과와 각 Collider마다 충돌체크를 하는 경우를 방지해본다.
	//		단. 이 경우 구조에 대해 생각을 조금 더 해봐야한다.

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * screenX / width - 1.0f) / matProj(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / matProj(1, 1);

	Matrix matView = camera->matView;
	Matrix matViewInv = matView.Invert();

	vector<Ref<GameObject>>& gameObjects = GetCurrentScene()->GetAllGameObjects();

	float minDistance = FLT_MAX;
	Ref<GameObject> picked;

	for (auto& go : gameObjects) {
		if (go->GetCollider() == nullptr) {
			continue;
		}

		Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
		Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);
	
		rayOrigin = XMVector3TransformCoord(rayOrigin, matViewInv);
		rayDir = XMVector3TransformNormal(rayDir, matViewInv);
		rayDir.Normalize();

		float distance = 0.0f;
		if (go->GetCollider()->Intersects(rayOrigin, rayDir, OUT distance) == false) {
			continue;
		}

		if (distance < minDistance) {
			minDistance = distance;
			picked = go;
		}
	}

	return picked;
}
