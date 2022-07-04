#pragma once
#include "MonoBehaviour.h"

class CameraController : public MonoBehaviour
{
public:
	/* ----- Constructor Function ----- */
	CameraController();
	virtual ~CameraController();

public:
	/* ----- virtual Function ----- */
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
public:
	/* ----- GUI Function ----- */
	virtual void DrawGUI();

	void TargetFollow();
	void CameraMove();

	void SetTarget(Ref<GameObject> target) { _target = target; }
	void SetOffset(const Vec3& offset) { _offset = offset; }
private:
	float _speed = 100.0f;

	Ref<GameObject> _target;
	Vec3 _offset;
};

