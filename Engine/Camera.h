#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,		// ��������
	ORTHOGRAPHIC,		// ��������
};

/*-----------
	Camera
------------*/
class Camera : public Component
{
public:
	/* ----- Constroct Function ----- */
	Camera();
	virtual ~Camera();

	/* ----- External Function ----- */
	virtual void		FinalUpdate() override;
	//void				Render();

	void				SortGameObject();
	void				SortForwadObject();
	void				SortShadowGameObject();
	void				SortPickGameObject();

	void				Render_Deferred();
	void				Render_Forward();
	void				Render_Shadow();
	void				Render_Pick();

	/* ----- Helper Function ----- */
	float				GetNear() { return _near; }
	float				GetFar() { return _far; }
	float				GetFov() { return _fov; }
	PROJECTION_TYPE		GetProjectionType() { return _type; }

	Matrix&				GetViewMatrix() { return _matView; }
	Matrix&				GetProjectionMatrix() { return _matProjection; }

	void				SetProjectionType(PROJECTION_TYPE type) { _type = type; }
	void				SetNear(float value) { _near = value; }
	void				SetFar(float value) { _far = value; }
	void				SetFov(float value) { if (value < 0.1f) { value = 0.1f; } else if (value >= XM_PI) { value = XM_PI; }_fov = value; }	// �����Ƽ� ���ٷ� �ڵ���
	void				SetScale(float value) { _scale = value; }
	void				SetWidth(float value) { _width = value; }	
	void				SetHeight(float value) { _height = value; }	
	
	void SetCullingMaskLayerOnOff(uint8 layer, bool flag)
	{
		if (flag) {
			_cullingMask |= (1 << layer);
		}
		else {
			_cullingMask &= ~(1 << layer);
		}
	}
	void SetCullingMaskLayerOnOff(LAYER_TYPE layer, bool flag) { SetCullingMaskLayerOnOff(static_cast<uint8>(layer),flag); }
	
	void				SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void				SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool				IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }
private:
	PROJECTION_TYPE		_type = PROJECTION_TYPE::PERSPECTIVE;

	float				_near = 1.0f;
	float				_far = 2000.0f;
	float				_fov = XM_PI / 4.0f;
	float				_scale = 1.0f;
	float				_width = 0.0f;
	float				_height = 0.0f;

	Matrix				_matView = {};
	Matrix				_matProjection = {};

	Frustum				_frustum;
	uint32				_cullingMask = 0;

	Ref<class Texture>	_PickTargetView;
private:
	vector<Ref<GameObject>> _vecDeferred;
	vector<Ref<GameObject>> _vecForward;
	vector<Ref<GameObject>> _vecParticle;
	vector<Ref<GameObject>> _vecShadow;
	vector<Ref<GameObject>> _vecPick;

public:
	static Matrix		S_MatView;			
	static Matrix		S_MatProjection;		// �ܺο��� ���� ���ϰ� ����༮
};
