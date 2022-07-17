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
	GetGameObject()->GetCollider()->AddBind(COLLIDER_STATE::LEAVE, 
		[=](Ref<BaseCollider> dest) { CollisionTest(dest); });


}

void PlayerController::Start()
{
	
}

void PlayerController::Update()
{
	RayCastHitInfo hit;
	if (INPUT->GetButton(KEY_TYPE::LBUTTON) == true) {
		if (GET_SINGLE(SceneManager)->Pick(INPUT->GetMousePos().x, INPUT->GetMousePos().y, hit) != nullptr) {
			m_point = hit.point;
		}
	}

	if (m_point != GetTransform()->localPosition) {
		Vec3 sub = m_point - GetTransform()->localPosition;
		sub.Normalize();
		GetTransform()->localPosition += sub * m_speed * DELTATIME;
	}
}

void PlayerController::LateUpdate()
{
}

void PlayerController::CollisionTest(Ref<class BaseCollider> dest)
{
	EDITOR->Log("Collision Test");
}

void PlayerController::EditorUpdate()
{
	if (ImGui::DragFloat("Speed", &m_speed, 1.0f, 0.0f, 1000.0f)) {
	}
	
}
