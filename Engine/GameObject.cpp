#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MonoBehaviour.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "BaseCollider.h"
#include "Animator.h"

uint32 GameObject::ID = 0;

GameObject::GameObject() : Object(OBJECT_TYPE::GAMEOBJECT)
{
	GameObject::ID += 1;
	static char nameText[50];
	::sprintf_s(nameText, 50, "Unkwon(%u)", GameObject::ID);
	name = Utils::Str2Wstr(nameText);
}

GameObject::~GameObject()
{
	for (Ref<Component>& component : _components) {
		if (component) {
			component = nullptr;
		}
	}
	for (auto& script : _scripts) {
		if (script.second) {
			script.second = nullptr;
		}
	}
	_scripts.clear();
}

void GameObject::Init()
{
	AddComponent(make_shared<Transform>());
}

void GameObject::Awake()
{

	for (const Ref<Component>& component : _components) {
		if (component) {
			component->Awake();
		}
	}
	for (const pair<string, Ref<MonoBehaviour>>& script : _scripts) {
		if (script.second) {
			script.second->Awake();
		}
	}
}

void GameObject::Start()
{
	if (m_isActive == false) {
		return;
	}

	for (const Ref<Component>& component : _components) {
		if (component) {
			component->Start();
		}
	}
	for (const pair<string, Ref<MonoBehaviour>>& script : _scripts) {
		if (script.second) {
			script.second->Start();
		}
	}


	for (const weak_ptr<Transform>& child : GetTransform()->GetChilds()) {
		const Ref<GameObject>& goChild = child.lock()->GetGameObject();
		goChild->Start();
	}
}

void GameObject::Update()
{
	if (m_isActive == false) {
		return;
	}

	for (const Ref<Component>& component : _components) {
		if (component) {
			component->Update();
		}
	}
	for (const pair<string, Ref<MonoBehaviour>>& script : _scripts) {
		if (script.second) {
			script.second->Update();
		}
	}

	for (const weak_ptr<Transform>& child : GetTransform()->GetChilds()) {
		const Ref<GameObject>& goChild = child.lock()->GetGameObject();
		goChild->Update();
	}
}

void GameObject::LateUpdate()
{
	if (m_isActive == false) {
		return;
	}

	for (const Ref<Component>& component : _components) {
		if (component) {
			component->LateUpdate();
		}
	}
	for (const pair<string, Ref<MonoBehaviour>>& script : _scripts) {
		if (script.second) {
			script.second->LateUpdate();
		}
	}

	for (const weak_ptr<Transform>& child : GetTransform()->GetChilds()) {
		const Ref<GameObject>& goChild = child.lock()->GetGameObject();
		goChild->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	if (m_isActive == false) {
		return;
	}

	for (const Ref<Component>& component : _components) {
		if (component) {
			component->FinalUpdate();
		}
	}

	for (const weak_ptr<Transform>& child : GetTransform()->GetChilds()) {
		const Ref<GameObject>& goChild = child.lock()->GetGameObject();
		goChild->FinalUpdate();
	}
}

void GameObject::Serializer(class Json::Value& scene)
{
	Json::Value& gameObject = scene[Utils::Wstr2Str(m_name)];

	for(auto& component : _components) {
		if (component) {
			component->Serializer(gameObject);
		}
	}

	for (auto& script : _scripts) {
		if (script.second) {
			script.second->Serializer(gameObject);
		}
	}
}

void GameObject::DeSerializer(class Json::Value& scene)
{
	Json::Value gameObject = scene[Utils::Wstr2Str(m_name)];

	for (auto& component : _components) {
		if (component) {
			component->Serializer(gameObject);
		}
	}

	for (auto& script : _scripts) {
		if (script.second) {
			script.second->Serializer(gameObject);
		}
	}
}

Ref<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

Ref<Transform> GameObject::GetTransform()
{
	return static_pointer_cast<Transform>(GetFixedComponent(COMPONENT_TYPE::TRANSFORM));
}

Ref<MeshRenderer> GameObject::GetMeshRenderer()
{
	return static_pointer_cast<MeshRenderer>(GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER));
}

Ref<Camera> GameObject::GetCamera()
{
	return static_pointer_cast<Camera>(GetFixedComponent(COMPONENT_TYPE::CAMERA));
}

Ref<Light> GameObject::GetLight()
{
	return static_pointer_cast<Light>(GetFixedComponent(COMPONENT_TYPE::LIGHT));
}

Ref<ParticleSystem> GameObject::GetParticleSystem()
{
	return static_pointer_cast<ParticleSystem>(GetFixedComponent(COMPONENT_TYPE::PARTICLE_SYSTEM));
}

Ref<Terrain> GameObject::GetTerrain()
{
	return static_pointer_cast<Terrain>(GetFixedComponent(COMPONENT_TYPE::TERRAIN));
}

Ref<BaseCollider> GameObject::GetCollider()
{
	return static_pointer_cast<BaseCollider>(GetFixedComponent(COMPONENT_TYPE::COLLIDER));
}

Ref<Animator> GameObject::GetAnimator()
{
	return static_pointer_cast<Animator>(GetFixedComponent(COMPONENT_TYPE::ANIMATOR));
}
