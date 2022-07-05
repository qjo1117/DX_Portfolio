#pragma once
#include "Component.h"

enum class LIGHT_TYPE : int32
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

// 까먹을까봐하는 건데 HLSL은 16byte 정렬을 기본으로 해서
// float3만 사용하지만 float4형식으로 맞춰준 것이다.

struct LightColor
{
	Vec4 diffuse;
	Vec4 ambient;
	Vec4 specular;
};

struct LightInfo
{
	LightColor	color;
	Vec4		position;
	Vec4		direction;

	int32		lightType;
	float		range;
	float		angle;
	int32		padding;
};

// 실제로 Shader코드에 입력될 구조체
struct LightParams
{
	uint32		lightCount;
	Vec3		padding;
	LightInfo	lights[50];
};

/*-----------------
	SceneManager
------------------*/
class Light : public Component
{
public:
	/* ----- Constructor Function ----- */
	Light();
	virtual ~Light();

	/* ----- virtual Function ----- */
	virtual void FinalUpdate() override;
	virtual void EditorUpdate() override;
	
	void Render();
	void RenderShadow();

public:
	/* ----- Helper Function ----- */
	const LightInfo& GetLightInfo() { return m_lightInfo; }

	void SetLightDirection(Vec3 direction);

	void SetDiffuse(const Vec3& diffuse) { m_lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { m_lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specualr) { m_lightInfo.color.specular = specualr; }

	const Vec4& GetDiffuse() { return m_lightInfo.color.diffuse; }
	const Vec4& GetAmbient() { return m_lightInfo.color.ambient; }
	const Vec4& GetSpecular() { return m_lightInfo.color.specular; }
	LIGHT_TYPE GetLightType() { return static_cast<LIGHT_TYPE>(m_lightInfo.lightType); }

	void SetLightType(LIGHT_TYPE type);
	void SetLightRange(float range) { m_lightInfo.range = range; }
	void SetLightAngle(float angle) { m_lightInfo.angle = angle; }

	const float& GetRange() { return m_lightInfo.range; }
	const float& GetAngle() { return m_lightInfo.angle; }

	void SetLightIndex(int8 index) { m_lightIndex = index; }


private:
	/* ----- LightInfo Variable ----- */
	PRIVATE_PROPERTY(LightInfo,		lightInfo) = {};

	PRIVATE_PROPERTY(int8,			lightIndex) = -1;
	Ref<class Mesh>					m_volumeMesh;
	Ref<class Material>				m_lightMaterial;
	Ref<class GameObject>			m_shadowCamera;

	Ref<class Texture>				m_shadowTex;
};

