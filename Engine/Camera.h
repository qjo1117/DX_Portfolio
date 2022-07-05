#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,		// 원근투영
	ORTHOGRAPHIC,		// 직교투영
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
	virtual void		EditorUpdate() override;
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

	void SetCullingMaskLayerOnOff(uint8 layer, bool flag)
	{
		if (flag) {
			m_cullingMask |= (1 << layer);
		}
		else {
			m_cullingMask &= ~(1 << layer);
		}
	}
	void SetCullingMaskLayerOnOff(LAYER_TYPE layer, bool flag) { SetCullingMaskLayerOnOff(static_cast<uint8>(layer),flag); }
	
	void				SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void				SetCullingMask(uint32 mask) { m_cullingMask = mask; }
	bool				IsCulled(uint8 layer) { return (m_cullingMask & (1 << layer)) != 0; }
private:

	PRIVATE_PROPERTY(PROJECTION_TYPE, CameraType)		= PROJECTION_TYPE::PERSPECTIVE;

	PRIVATE_PROPERTY(float, Near)				= 1.0f;
	PRIVATE_PROPERTY(float, Far)				= 2000.0f;
	PRIVATE_PROPERTY(float, fov)				= XM_PI / 4.0f;
	PRIVATE_PROPERTY(float, scale)				= 1.0f;
	PRIVATE_PROPERTY(float, width)				= 0.0f;
	PRIVATE_PROPERTY(float, height)				= 0.0f;

	PRIVATE_PROPERTY(Matrix, matView)			= Matrix::Identity;
	PRIVATE_PROPERTY(Matrix, matProjection)		= Matrix::Identity;

	PRIVATE_PROPERTY(Frustum, frustum);
	PRIVATE_PROPERTY(uint32, cullingMask) = 0;

	Ref<class Texture>	_PickTargetView;
private:
	vector<Ref<GameObject>> _vecDeferred;
	vector<Ref<GameObject>> _vecForward;
	vector<Ref<GameObject>> _vecParticle;
	vector<Ref<GameObject>> _vecShadow;
	vector<Ref<GameObject>> _vecPick;

public:
	static Matrix		S_MatView;			
	static Matrix		S_MatProjection;		// 외부에서 쓰기 편하게 만든녀석
};

