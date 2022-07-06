#include "pch.h"
#include "BaseCollider.h"
#include "Resources.h"
#include "Material.h"


BaseCollider::BaseCollider(COLLIDER_TYPE type) :
	Component(COMPONENT_TYPE::COLLIDER),
	m_ColliderType(type)
{
	material = GET_SINGLE(Resources)->Get<Material>(L"Wirefram");
}

BaseCollider::~BaseCollider()
{
}

