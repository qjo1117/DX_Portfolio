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

void BaseCollider::End()
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
		m_BindEnterFunc.push_back(func);
	}
	else if (state == COLLIDER_STATE::PRESS) {
		m_BindPressFunc.push_back(func);
	}
	else if (state == COLLIDER_STATE::LEAVE) {
		m_BindLeaveFunc.push_back(func);
	}
}