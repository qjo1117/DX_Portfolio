#pragma once

#include "MonoBehaviour.h"

/*----------------------
	PlayerController
-----------------------*/
class PlayerController : public MonoBehaviour
{
public:
	/* ----- Constructor Function ----- */
	PlayerController();
	virtual ~PlayerController();

public:
	/* ----- virtual Function ----- */
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	void CollisionTest(Ref<class BaseCollider> dest);

public:
	/* ----- GUI Function ----- */
	virtual void EditorUpdate();


	float GetSpeed() { return m_speed; }
	void SetSpeed(float speed) { m_speed = speed; }
private:
	float m_speed = 100.0f;
	
	bool m_isGround = false;

	Vec3 m_point = Vec3::Zero;

	Ref<class GameObject> m_castCheck;
};

