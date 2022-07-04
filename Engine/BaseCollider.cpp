#include "pch.h"
#include "BaseCollider.h"

BaseCollider::BaseCollider(ColliderType type) :
	Component(COMPONENT_TYPE::COLLIDER),
	_colliderType(type)
{

}

BaseCollider::~BaseCollider()
{
}
