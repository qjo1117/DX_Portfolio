#pragma once

#include "MonoBehaviour.h"
#include "Component.h"
#include "Object.h"
#include "Resources.h"

#include "ColliderManager.h"

class Component;
class Transform;
class MeshRenderer;
class MonoBehaviour;
class Camera;
class Light;
class ParticleSystem;
class Terrain;
class BaseCollider;
class Rigidbody;
class Animator;

/*---------------
	GameObject
----------------*/
class GameObject : public Object, public enable_shared_from_this<GameObject>
{
public:
	/* ----- Constructor Function ----- */
	GameObject();
	virtual ~GameObject();

	/* ----- FrameWork Function ----- */
	void Init();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	/* ----- Serializer Function ------ */
	virtual void Serializer(class Json::Value& scene);
	virtual void DeSerializer(class Json::Value& scene);

	/* ----- Helper Component Function ----- */
	inline Ref<Component> GetFixedComponent(COMPONENT_TYPE type);

	Ref<Transform>		GetTransform();
	Ref<MeshRenderer>	GetMeshRenderer();
	Ref<Camera>			GetCamera();
	Ref<Light>			GetLight();
	Ref<ParticleSystem>	GetParticleSystem();
	Ref<Terrain>		GetTerrain();
	Ref<BaseCollider>	GetCollider();
	Ref<Rigidbody>		GetRigidbody();
	Ref<Animator>		GetAnimator();

	map<string, Ref<MonoBehaviour>>& GetScripts() { return m_scripts; }

	template<typename T>
	void AddComponent(Ref<T> component);
	template<typename T>
	Ref<T> GetComponent();
	template<typename T>
	Ref<T> GetOrAddComponent();

	vector<Ref<Component>> GetAllComponent()
	{
		vector<Ref<Component>> components;

		for (Ref<Component>& component : m_components) {
			if (component != nullptr) {
				components.push_back(component);
			}
		}

		for (auto& itme : m_scripts) {
			components.push_back(itme.second);
		}

		return components;
	}

	/* ----- Helper Function ----- */
	void			SetLayer(uint32 type) { m_layerType = type; }
	void			SetLayer(LAYER_TYPE type) { SetLayer(static_cast<uint32>(type)); }

private:
	PRIVATE_PROPERTY(uint32, layerType) = static_cast<uint32>(LAYER_TYPE::DEFAULT);

	/* ----- Component Variable ----- */
	array<Ref<Component>, FIXED_COMPONENT_COUNT> m_components;
	map<string, Ref<MonoBehaviour>> m_scripts;

	PRIVATE_PROPERTY(bool, isFrustum) = true;
	PRIVATE_PROPERTY(bool, isActive) = true;
	PRIVATE_PROPERTY(bool, isShadow) = true;
public:
	static uint32 ID;
};

template<typename T>
inline void GameObject::AddComponent(Ref<T> component)
{
	if (!std::is_convertible_v<T, Component>) {
		return;
	}

	/* ----- 컴포넌트의 주인을 알려주고 맵핑번호가 넘어가면 MonoBehaviour라고 취급 ----- */
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT) {
		m_components[index] = component;
		if (index == static_cast<uint8>(COMPONENT_TYPE::COLLIDER)) {
			GET_SINGLE(ColliderManager)->AddCollider(static_pointer_cast<BaseCollider>(GetFixedComponent(COMPONENT_TYPE::COLLIDER)));
		}
	}
	else {
		static char* namePointer = (char*)(&typeid(component).name()[28]);
		namePointer[::strlen(namePointer) - 1] = '\0';
		m_scripts.insert(make_pair<string, Ref<MonoBehaviour>>(namePointer, dynamic_pointer_cast<MonoBehaviour>(component)));
	}
}

template<typename T>
inline Ref<T> GameObject::GetComponent()
{
	uint8 index = static_cast<uint8>(objectType);

	if (index < FIXED_COMPONENT_COUNT) {
		return static_pointer_cast<T>(m_components[index]);
	}

	auto findIt = m_scripts.find((char*)(&typeid(T).name()[6]));

	if (findIt == m_scripts.end()) {
		return nullptr;
	}
	return static_pointer_cast<T>(findIt->second);
}

template<typename T>
inline Ref<T> GameObject::GetOrAddComponent()
{
	Ref<T> component = GetComponent<T>();
	if (component == nullptr) {
		component = make_shared<T>();
		AddComponent(component);
	}
	return component;
}