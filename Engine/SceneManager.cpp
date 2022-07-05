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
	/* ----- Scene�� �����ϴ��� üũ�ϰ� ������ ����ȯ�� �Ѵ�. ----- */
	assert(_scenes[static_cast<uint32>(type)]);

	if (_currentScene == nullptr) {
		_currentScene = _scenes[static_cast<uint32>(type)];
	}
	else {
		// TODO : ���빰 �ʱ�ȭ
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

	// ������ �����ؾ��� ��
	// 1.	World Space������ ��ȯ���� ������. ���� Invert�ϴ� Matrix�� Intersects�� �ִ� ��ǥ��
	//		Local Space�� �ٲ���� Ȯ���ϴµ� ���� �� ����.
	//		���� : MeshCollider���� ����� �־��� ��� �� �ﰢ���� ��ǥ�� float���� �Ѿ�� ��찡 �����.
	// 2.	ColliderManager�� ���� �浹üũ ������ ���� ����� �ش�.
	//		�����Ӻ��� �ð��� �����ִ� ȿ���� �� Collider���� �浹üũ�� �ϴ� ��츦 �����غ���.
	//		��. �� ��� ������ ���� ������ ���� �� �غ����Ѵ�.

	// ViewSpace���� Picking ����
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
