#pragma once

#include "MonoBehaviour.h"
#include "Component.h"
#include "Object.h"
#include "Resources.h"

class Component;
class Transform;
class MeshRenderer;
class MonoBehaviour;
class Camera;
class Light;
class ParticleSystem;
class Terrain;
class BaseCollider;
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
	Ref<Animator>		GetAnimator();

	map<string, Ref<MonoBehaviour>>& GetScripts() { return _scripts; }

	template<typename T>
	void AddComponent(Ref<T> component);
	template<typename T>
	Ref<T> GetComponent();
	template<typename T>
	Ref<T> GetOrAddComponent();

	vector<Ref<Component>> GetAllComponent()
	{
		vector<Ref<Component>> components;

		for (Ref<Component>& component : _components) {
			if (component != nullptr) {
				components.push_back(component);
			}
		}

		for (auto& itme : _scripts) {
			components.push_back(itme.second);
		}

		return components;
	}

	/* ----- Helper Function ----- */
	uint32			GetLayer() { return _layerType; }
	void			SetLayer(uint32 type) { _layerType = type; }
	void			SetLayer(LAYER_TYPE type) { SetLayer(static_cast<uint32>(type)); }

	bool			GetCheckFrustum() { return _isFrustum; }
	void			SetCheckFrustum(bool check) { _isFrustum = check; }

	inline bool		GetActive() { return _isActive; }
	void			SetActive(bool active) { _isActive = active; }

	void			SetShadow(bool flag) { _isShadow = flag; }
	bool			IsShadow() { return _isShadow; }
private:
	uint32 _layerType = static_cast<uint32>(LAYER_TYPE::DEFAULT);

	/* ----- Component Variable ----- */
	array<Ref<Component>, FIXED_COMPONENT_COUNT> _components;
	map<string, Ref<MonoBehaviour>> _scripts;

	bool _isFrustum = true;

	bool _isActive = true;
	bool _isShadow = true;
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
		_components[index] = component;
	}
	else {
		static char* namePointer = (char*)(&typeid(component).name()[28]);
		namePointer[::strlen(namePointer) - 1] = '\0';
		_scripts.insert(make_pair<string, Ref<MonoBehaviour>>(namePointer, dynamic_pointer_cast<MonoBehaviour>(component)));
	}
}

template<typename T>
inline Ref<T> GameObject::GetComponent()
{
	uint8 index = static_cast<uint8>(GetType());

	if (index < FIXED_COMPONENT_COUNT) {
		return static_pointer_cast<T>(_components[index]);
	}

	auto findIt = _scripts.find((char*)(&typeid(T).name()[6]));

	if (findIt == _scripts.end()) {
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