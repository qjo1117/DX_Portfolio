#pragma once
#include "Scene.h"


/*--------------
	ToolScene
---------------*/
class ToolScene : public Scene
{
public:
	/* ----- Constract Function ----- */
	ToolScene();
	virtual ~ToolScene();

public:
	/* ----- External Function ----- */
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate() override;

	void	Create();

public:
	/* ----- Test or GUI Function ----- */
	void	TestEditor();
	void	TestObjects();
	void	TestResources();
	void	TestRenderTargetView();

	void	TestButtonCreateObject();
	void	TestEditorWindow();

	/* ----- 너무 길어서 각 컴포넌트 별로 함수를 만들어줌 ----- */
	void	TransformComponent();
	void	MeshRendererComponent();
	void	CameraComponent();
	void	LightComponent();
	void	ParticleComponent();
	void	TerrainComponent();

	void	ImGuiMaterials(Ref<class MeshRenderer>& renderer);
	void	ImGuiMesh(Ref<class MeshRenderer>& renderer);

private:
	void	RemoveSelectObject();
	void	CreateCubeGameObject();

	void	SceneSerializered();
	void	DeSceneSerializered();

private:
	void	ParentTree(const Ref<GameObject>& obj);

private:
	/* ----- Fick Variable ----- */
	Ref<class GameObject>		_goPick;
	Ref<class Object>			_objPick;
	int32						_isPick = -1;
	Ref<GameObject>				_dragPick;

	Ref<GameObject>				_player;
	Ref<GameObject>				_camera;
	Ref<class Texture>			_pickTexture;

	vector<Vec2>				_pickTestDraw;

	Random						_random;
	vector<function<void()>>	_createList;

	vector<Ref<GameObject>>		_renderTargetView;

	char						_text[MAX_PATH];
	string						_strName;
	Ref<class Texture>			_defauleImage;

	Ref<GameObject>				_tessellation;
};
