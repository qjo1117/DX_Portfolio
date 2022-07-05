#pragma once

#include "Object.h"

class GameObject;
class Transform;

/*--------------
	Component
---------------*/
class Component : public Object
{
public:
	/* ----- Constructor Function ----- */
	Component(COMPONENT_TYPE type);
	virtual ~Component();

public:
	/* ----- Virtual Function ----- */
	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FinalUpdate() { }

	virtual void EditorUpdate() { }

	/* ----- Serializer Function ------ */
	virtual void Serializer(class Json::Value& object) {}
	virtual void DeSerializer(class Json::Value& object) {}

public:
	/* ----- Helper Function ----- */
	COMPONENT_TYPE			GetType() { return _type; }
	bool					IsValid() { return _gameObject.expired() == false; }

	Ref<GameObject>	GetGameObject();
	Ref<Transform>	GetTransform();

private:
	/* ----- Friend Helper Function ----- */
	friend class GameObject;
	void SetGameObject(Ref<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	/* ----- Component Variable ----- */
	COMPONENT_TYPE			_type;
	weak_ptr<GameObject>	_gameObject;
};

