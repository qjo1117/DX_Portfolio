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

Ref<class GameObject> SceneManager::Pick(int32 screenX, int32 screenY, RayCastHitInfo& hit)
{
	Ref<Camera> camera = GetCurrentScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix matProj = camera->matProjection;

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * screenX / width - 1.0f) / matProj(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / matProj(1, 1);

	Matrix matView = camera->matView;
	Matrix matViewInv = matView.Invert();

	Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

	rayOrigin = XMVector3TransformCoord(rayOrigin, matViewInv);
	rayDir = XMVector3TransformNormal(rayDir, matViewInv);
	rayDir.Normalize();

	float minDistance = FLT_MAX;
	Ref<GameObject> picked;
	if (GET_SINGLE(ColliderManager)->RayCast(rayOrigin, rayDir, OUT hit, LAYER_TYPE::END, 10000.0f)) {
		picked = hit.collider->GetGameObject();
	}

	return picked;
}

Ref<class GameObject> SceneManager::Pick(int32 screenX, int32 screenY)
{
	Ref<Camera> camera = GetCurrentScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix matProj = camera->matProjection;

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * screenX / width - 1.0f) / matProj(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / matProj(1, 1);

	Matrix matView = camera->matView;
	Matrix matViewInv = matView.Invert();

	Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

	rayOrigin = XMVector3TransformCoord(rayOrigin, matViewInv);
	rayDir = XMVector3TransformNormal(rayDir, matViewInv);
	rayDir.Normalize();
	float minDistance = FLT_MAX;
	Ref<GameObject> picked;

	float distance = 0.0f;
	for (auto collider : GET_SINGLE(ColliderManager)->GetCollider()) {
		if (collider->Intersects(rayOrigin, rayDir, distance) == true) {
			if (distance < minDistance) {
				picked = collider->GetGameObject();
				minDistance = distance;
			}
		}
	}

	return picked;
}
