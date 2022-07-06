#include "pch.h"
#include "PlayerController.h"
#include "Engine.h"
#include "Transform.h"
#include "EditorManager.h"
#include "BaseCollider.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Awake()
{
	GetGameObject()->GetCollider()->BindFunc.push_back([=](Ref<BaseCollider> dest) { CollisionTest(dest); });
}

void PlayerController::Start()
{
	
}

void PlayerController::Update()
{
	if (INPUT->IsAnyKey()) {
		return;
	}

	Vec3 move = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W)) {
		move.z += _speed * DELTATIME;
	}
	if (INPUT->GetButton(KEY_TYPE::S)) {
		move.z -= _speed * DELTATIME;
	}
	if (INPUT->GetButton(KEY_TYPE::A)) {
		move.x -= _speed * DELTATIME;
	}
	if (INPUT->GetButton(KEY_TYPE::D)) {
		move.x += _speed * DELTATIME;
	}
	GetTransform()->SetLocalPosition(move);

	move = Vec3(0.0f, 0.0f, 0.0f);
	if (INPUT->GetButton(KEY_TYPE::Q)) {
		move.x += 1.0f;
	}
	if (INPUT->GetButton(KEY_TYPE::E)) {
		move.x -= 1.0f;
	}
	GetTransform()->SetLocalRotation(move);
}

void PlayerController::LateUpdate()
{
}

void PlayerController::CollisionTest(Ref<class BaseCollider> dest)
{
	EDITOR->Log("Collision");
	Vec3 dir = dest->GetTransform()->localPosition - GetTransform()->localPosition;
	dir.Normalize();
	GetTransform()->localPosition -= dir;
}

void PlayerController::EditorUpdate()
{
	if (ImGui::DragFloat("Speed", &_speed, 1.0f, 0.0f, 1000.0f)) {
		_speed = _speed;
	}
	
}
