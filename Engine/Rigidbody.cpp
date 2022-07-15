#include "pch.h"
#include "Rigidbody.h"
#include "GameObject.h"
#include "BaseCollider.h"
#include "Transform.h"

Rigidbody::Rigidbody() : Component(COMPONENT_TYPE::RIGIDBODY)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Awake()
{
	// 콜라이더가 있을경우
	if (GetGameObject()->GetCollider() != nullptr) {
		GetGameObject()->GetCollider()->AddBind(
			COLLIDER_STATE::ENTER, 
			[self = GetGameObject()] (Ref<BaseCollider> collider) { 
			dynamic_pointer_cast<Rigidbody>(self)->RigidbodyCollision(collider); 
		});

		m_collider = GetGameObject()->GetCollider();
	}
}

void Rigidbody::Update()
{

}

void Rigidbody::FinalUpdate()
{

}

void Rigidbody::RigidbodyCollision(Ref<class BaseCollider> p_collider)
{

}

void Rigidbody::AddForce(Vec3 p_force)
{
}
