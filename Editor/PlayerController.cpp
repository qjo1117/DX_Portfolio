#include "pch.h"
#include "PlayerController.h"
#include "Engine.h"
#include "Transform.h"
#include "EditorManager.h"
#include "BaseCollider.h"

#include "Terrain.h"


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
	//RayCastHitInfo hit;
	//m_isGround = false;
	//if (GET_SINGLE(ColliderManager)->RayCast(GetTransform()->localPosition, -Vec3::Up, hit, LAYER_TYPE::GROUND, 50.0f) == true) {
	//	m_isGround = true;
	//}

	//if (m_isGround == false) {
	//	GetTransform()->localPosition -= Vec3::Up * 9.8f * DELTATIME;
	//}
}

void PlayerController::LateUpdate()
{
}

void PlayerController::CollisionTest(Ref<class BaseCollider> dest)
{
	EDITOR->Log("Collision");
}

void PlayerController::EditorUpdate()
{
	if (ImGui::DragFloat("Speed", &_speed, 1.0f, 0.0f, 1000.0f)) {
		_speed = _speed;
	}
	
}
