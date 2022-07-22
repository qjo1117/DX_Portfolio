#include "pch.h"
#include "Rigidbody.h"
#include "GameObject.h"
#include "BaseCollider.h"
#include "Transform.h"
#include "Timer.h"

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
			[self = GetGameObject()->shared_from_this()](Ref<BaseCollider> collider) {
			self->GetRigidbody()->RigidbodyCollision(collider);
		});

		m_collider = GetGameObject()->GetCollider();
	}
}

void Rigidbody::Update()
{

}

void Rigidbody::FinalUpdate()
{
	if (m_force != Vec3::Zero) {
		// 현재 가속도를 구합니다.
		Vec3 accel = m_force * DELTATIME;
		m_velocity = accel * m_mass;
		force = Vec3::Zero;
	}

	if (m_velocity != Vec3::Zero) {
		Vec3 vFriction = -m_velocity;
		vFriction.Normalize();
		vFriction *= m_friction;

		if (vFriction.LengthSquared() < m_velocity.LengthSquared()) {
			m_velocity += vFriction;
		}
		else {
			m_velocity = Vec3::Zero;
		}
		GetTransform()->localPosition += m_velocity;
	}
}

void Rigidbody::RigidbodyCollision(Ref<class BaseCollider> p_collider)
{
	Ref<Rigidbody> targetRigidbody = p_collider->GetGameObject()->GetRigidbody();
	Vec3 subVec = GetTransform()->localPosition - p_collider->GetTransform()->localPosition;
	Vec3 velocity = m_velocity;
	m_velocity.x = m_e * -velocity.x / m_mass;
	m_velocity.y = m_e * -velocity.y / m_mass;
	m_velocity.z = m_e * -velocity.z / m_mass;

	if (targetRigidbody != nullptr) {
		targetRigidbody->velocity.x = targetRigidbody->m_e * velocity.x / targetRigidbody->mass;
		targetRigidbody->velocity.y = targetRigidbody->m_e * velocity.y / targetRigidbody->mass;
		targetRigidbody->velocity.z = targetRigidbody->m_e * velocity.z / targetRigidbody->mass;
	}
}

void Rigidbody::AddForce(Vec3 p_force)
{
	m_force += p_force;
}
