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

public:
	/* ----- GUI Function ----- */
	virtual void EditorUpdate();


	float GetSpeed() { return _speed; }
	void SetSpeed(float speed) { _speed = speed; }
private:
	float _speed = 100.0f;

};

