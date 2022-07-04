#include "pch.h"
#include "Billboard.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"


Billboard::Billboard()
{
}

Billboard::~Billboard()
{

}

void Billboard::Awake()
{
	// Camera Index에 첫번째 있는 오브젝트가 메인이라고 생각한다.
	_camera = GET_SINGLE(SceneManager)->GetCurrentScene()->GetGameObjects(LAYER_TYPE::CAMERA)[0];

}

void Billboard::Start()
{
	
}

void Billboard::Update()
{
	if (_camera.lock()) {
		GetTransform()->LookAt(_camera.lock()->GetTransform()->GetLocalPosition());
	}
}

void Billboard::LateUpdate()
{
}

void Billboard::DrawGUI()
{
}
