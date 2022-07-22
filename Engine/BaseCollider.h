#pragma once

#include "Component.h"

enum class COLLIDER_STATE
{
	ENTER,
	PRESS,
	LEAVE,
	END
};

enum class COLLIDER_TYPE
{
	SPHERE,
	BOX,
	MESH,
	END
};


struct ColliderState
{
public:
	bool Enter = false;
	bool Press = false;
	bool Leave = false;
};

struct RayCastHitInfo
{
	Vec3 point = Vec3::Zero;
	Vec3 normal = Vec3::Zero;
	float distance = 0.0f;
	Ref<class BaseCollider> collider = nullptr;
};

class BaseCollider : public Component
{
public:
	BaseCollider(COLLIDER_TYPE type);
	virtual ~BaseCollider();

	void End();

	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) = 0;
	virtual bool Collision(Ref<class BaseCollider> collider) = 0;
	virtual void EditorUpdate() override {}

	void AddBind(COLLIDER_STATE state, function<void(Ref<class BaseCollider>)> func);

protected:
	PROTECTED_PROPERTY(Vec3, Center) = Vec3::Zero;
	PROTECTED_PROPERTY(COLLIDER_TYPE, ColliderType) = { };
	PROTECTED_PROPERTY(BoundingBox, Bound) = { Vec3::Zero, Vec3::One };
	PROTECTED_PROPERTY(ColliderState, State);
	PROTECTED_PROPERTY(bool, Check) = false;
	PROTECTED_PROPERTY(vector<function<void(Ref<BaseCollider>)>>, BindEnterFunc);
	PROTECTED_PROPERTY(vector<function<void(Ref<BaseCollider>)>>, BindPressFunc);
	PROTECTED_PROPERTY(vector<function<void(Ref<BaseCollider>)>>, BindLeaveFunc);
};

