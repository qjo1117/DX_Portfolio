#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Component::Component(COMPONENT_TYPE type) : 
	_type(type),
	Object(OBJECT_TYPE::COMPONENT)
{
}

Component::~Component()
{
}

Ref<GameObject> Component::GetGameObject()
{
	return _gameObject.lock();
}

Ref<Transform> Component::GetTransform()
{
	return _gameObject.lock()->GetTransform();
}
