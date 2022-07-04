#pragma once 

#include "Monobehaviour.h"



class Billboard : public MonoBehaviour
{
public:
	/* ----- Constructor Function ----- */
	Billboard();
	virtual ~Billboard();

public:
	/* ----- virtual Function ----- */
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

public:
	/* ----- GUI Function ----- */
	virtual void DrawGUI();

private:
	weak_ptr<class GameObject>	_camera;
	
	PUBLIC_PROPERTY(int32, Hp);		// ��ġ�� �������� �˷��ֽŹ��
};

