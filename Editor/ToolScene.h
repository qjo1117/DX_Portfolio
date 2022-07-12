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

private:
	/* ----- Fick Variable ----- */
	Ref<class GameObject> _camera;
	Ref<class GameObject> _pickSphere;
	Ref<class Texture> _defauleImage;
	Ref<class Texture> _pickTexture;
	vector<Ref<class GameObject>> _renderTargetView;
	string _strName;
};
