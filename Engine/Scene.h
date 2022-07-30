#pragma once

class GameObject;

/*----------
	Scene
-----------*/
class Scene : public enable_shared_from_this<Scene>
{
public:
	/* ----- Constructor Function ----- */
	Scene();
	virtual ~Scene();

public:
	/* ----- External Function ----- */
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

	void Render();

	void End();

	void ClearRTV();

	void RenderShadow();
	void RenderPick();
	void RenderDeferred();
	void RenderLights();
	void RenderFinal();

	void RenderFoward();

	void SetCullingMaskLayerOnOff(uint8 layer, bool flag)
	{
		if (flag) {
			_cullingMask |= (1 << layer);
		}
		else {
			_cullingMask &= ~(1 << layer);
		}
	}
	void SetCullingMaskLayerOnOff(LAYER_TYPE layer, bool flag) { SetCullingMaskLayerOnOff(static_cast<uint8>(layer), flag); }
	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

	Ref<class Camera> GetMainCamera();


	void SortSceneObjects();
	void ClearRenderList();

	Ref<GameObject> FindGameObject(const wstring& name);

private:
	void PushLightData();

public:
	/* ----- Helper Function ----- */
	void AddGameObject(Ref<GameObject> gameObject, LAYER_TYPE layer = LAYER_TYPE::DEFAULT);
	void RemoveObject(Ref<GameObject> gameObject, LAYER_TYPE layer = LAYER_TYPE::END);


	const wstring& GetName() { return _name; }
	void SetName(const wstring& name) { _name = name; }

	array<vector<Ref<GameObject>>, static_cast<uint32>(LAYER_TYPE::END)>& GetGameObjects() { return _gameObjects; }
	vector<Ref<GameObject>>&	GetGameObjects(LAYER_TYPE type) { return _gameObjects[static_cast<int32>(type)]; }
	vector<Ref<GameObject>>&	GetAllGameObjects() { return m_objects; }

	vector<Ref<GameObject>>&	GetDeferred() { return _vecDeferred; }
	vector<Ref<GameObject>>&	GetForward() { return _vecForward; }
	vector<Ref<GameObject>>&	GetParticle() { return _vecParticle; }
	vector<Ref<GameObject>>&	GetShadow() { return _vecShadow; }
	vector<Ref<GameObject>>&	GetRender() { return _vecRender; }
	vector<Ref<GameObject>>&	GetActived() { return _vecActived; }
	vector<Ref<GameObject>>&	GetNonActived() { return _vecNonActived; }

protected:
	/* ----- Helper Function ----- */
	bool RemoveObject(Ref<GameObject> gameObject, uint32 layer);
	void AddGameObject(Ref<GameObject> gameObject, uint32 layer);

	Ref<GameObject> Instantiate();

private:
	/* ----- ParantSort Function ----- */
	void	SortParantGameObject();

private:
	/* ----- Scene Object Mapping Variable ----- */
	array<vector<Ref<GameObject>>, static_cast<uint32>(LAYER_TYPE::END)> _gameObjects;
	vector<Ref<GameObject>>		m_objects;		// 한번에 전체순회
	vector<Ref<GameObject>>		_parentBaseObjects;
	wstring						_name;

	vector<Ref<GameObject>>&	_light;
	vector<Ref<GameObject>>&	_camera;
	uint32						_layerSize = static_cast<uint32>(LAYER_TYPE::END);
	uint32						_cullingMask = 0;

private:
	vector<Ref<GameObject>>		_vecDeferred;
	vector<Ref<GameObject>>		_vecForward;
	vector<Ref<GameObject>>		_vecParticle;
	vector<Ref<GameObject>>		_vecPick;
	vector<Ref<GameObject>>		_vecRender;
	vector<Ref<GameObject>>		_vecShadow;
	vector<Ref<GameObject>>		_vecActived;
	vector<Ref<GameObject>>		_vecNonActived;
};

