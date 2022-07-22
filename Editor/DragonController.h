#pragma once

#include "MonoBehaviour.h"


class DragonController : public MonoBehaviour
{
public:

	GENERATE_CLASS_TYPE_INFO(DragonController)

public:

	virtual void Update() override;

public:
	METHOD(Function)
	void Function();

	PROPERTY(speed)
	float m_speed = 100.0f;

	PROPERTY(isCheck)
	bool m_isCheck = false;

	PROPERTY(castCheck)
	Ref<class GameObject> m_castCheck;
};

