#pragma once

#include "Component.h"

enum class ColliderType
{
	Sphere,
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType type);
	virtual ~BaseCollider();

	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) = 0;

	void SetCenter(const Vec3& center) { _center = center; }
	const Vec3& GetCenter() { return _center; }

protected:
	ColliderType _colliderType = {};

	Vec3 _center = Vec3(0.0f, 0.0f, 0.0f);

};

