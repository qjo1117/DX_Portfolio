#include "pch.h"
#include "PlayerController.h"
#include "Engine.h"
#include "Transform.h"
#include "EditorManager.h"
#include "BaseCollider.h"

#include "Terrain.h"
#include "Rigidbody.h"


PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Awake()
{
	GetGameObject()->GetCollider()->AddBind(COLLIDER_STATE::LEAVE, 
		[self = GetGameObject()->shared_from_this()](Ref<BaseCollider> dest) { self->GetComponent<PlayerController>()->CollisionTest(dest); });
}

void PlayerController::Start()
{
	
}

void PlayerController::Update()
{
	//RayCastHitInfo hit;
	//if (INPUT->GetButton(KEY_TYPE::LBUTTON) == true) {
	//	if (GET_SINGLE(SceneManager)->Pick(INPUT->GetMousePos().x, INPUT->GetMousePos().y, hit) != nullptr) {
	//		m_point = hit.point;
	//	}
	//}

	//if (m_point != GetTransform()->localPosition) {
	//	Vec3 sub = m_point - GetTransform()->localPosition;
	//	sub.Normalize();
	//	GetTransform()->localPosition += sub * m_speed * DELTATIME;
	//}

	if (INPUT->GetButtonDown(KEY_TYPE::SPACE) == true) {
		GetGameObject()->GetRigidbody()->AddForce(-Vec3::Forward * 1000.0f);
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
	MonoBehaviour::EditorUpdate();
}
