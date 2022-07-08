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
	GetGameObject()->GetCollider()->AddBind(COLLIDER_STATE::PRESS, 
		[=](Ref<BaseCollider> dest) { CollisionTest(dest); });


}

void PlayerController::Start()
{
	
}

void PlayerController::Update()
{
	RayCastHit hit;
	if (GET_SINGLE(ColliderManager)->RayCast(GetTransform()->localPosition + Vec3::Forward * 110.0f, Vec3::Forward, hit, 100.0f) == true) {
		string str = "RayCast";
		EDITOR->Log(str);
	}
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
