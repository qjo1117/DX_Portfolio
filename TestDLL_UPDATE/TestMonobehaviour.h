#pragma once

#include "../Engine/MonoBehaviour.h"

class TestMonobehaviour :
    public MonoBehaviour
{
	GENERATE_CLASS_TYPE_INFO(TestMonobehaviour);

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

private:
	PROPERTY(count);
	float m_count = 0;
};

