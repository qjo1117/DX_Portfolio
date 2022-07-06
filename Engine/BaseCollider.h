#pragma once

#include "Component.h"

enum class COLLIDER_TYPE
{
	SPHERE,
	BOX,
	END
};

class BaseCollider : public Component
{
public:
	BaseCollider(COLLIDER_TYPE type);
	virtual ~BaseCollider();

	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) = 0;
	virtual bool Collision(Ref<BaseCollider> collider) = 0;
	virtual void EditorUpdate() override {}

	void SetCenter(const Vec3& center) { _center = center; }
	const Vec3& GetCenter() { return _center; }

protected:
	Vec3 _center = Vec3(0.0f, 0.0f, 0.0f);
	PROTECTED_PROPERTY(COLLIDER_TYPE, ColliderType) = { };
	PROTECTED_PROPERTY(BoundingBox, Bound) = { Vec3::Zero, Vec3::One };
	PROTECTED_PROPERTY(vector<function<void(Ref<BaseCollider>)>>, BindFunc);

	PROTECTED_PROPERTY(Ref<class Material>, material);
	PROTECTED_PROPERTY(Ref<class Mesh>, mesh);
};

