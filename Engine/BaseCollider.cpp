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
	for (function<void(Ref<BaseCollider>)> func : m_BindEnterFunc) {
		func = nullptr;
	}
	for (function<void(Ref<BaseCollider>)> func : m_BindPressFunc) {
		func = nullptr;
	}
	for (function<void(Ref<BaseCollider>)> func : m_BindLeaveFunc) {
		func = nullptr;
	}
	m_BindEnterFunc.clear();
	m_BindPressFunc.clear();
	m_BindLeaveFunc.clear();
}

void BaseCollider::AddBind(COLLIDER_STATE state, function<void(Ref<class BaseCollider>)> func)
{
	if (state == COLLIDER_STATE::ENTER) {
		BindEnterFunc.push_back(func);
	}
	else if (state == COLLIDER_STATE::PRESS) {
		BindPressFunc.push_back(func);
	}
	else if (state == COLLIDER_STATE::LEAVE) {
		BindLeaveFunc.push_back(func);
	}
}