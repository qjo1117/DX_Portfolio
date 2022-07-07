#include "pch.h"
#include "BaseCollider.h"
#include "Transform.h"


BaseCollider::BaseCollider(COLLIDER_TYPE type) :
	Component(COMPONENT_TYPE::COLLIDER),
	m_ColliderType(type)
{
}

BaseCollider::~BaseCollider()
{
}

