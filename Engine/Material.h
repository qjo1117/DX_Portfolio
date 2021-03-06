#pragma once

#include "Object.h"

enum
{
	MATERIAL_ARG_COUNT = 4,
};

struct MaterialParam
{
	MaterialParam() {
		for (int32 i = 0; i < MATERIAL_ARG_COUNT; ++i) {
			SetInt(i, 0);
			SetFloat(i, 0.0f);
			SetTexOn(i, 0);
		}
	}

	void SetInt(uint8 index, int32 value) { intParam[index] = value; }
	void SetFloat(uint8 index, float value) { floatParam[index] = value; }
	void SetTexOn(uint8 index, int32 value) { texOnParam[index] = value; }
	void SetVec2(uint8 index, Vec2 value) { vec2Param[index] = value; }
	void SetVec4(uint8 index, Vec4 value) { vec4Param[index] = value; }
	void SetMatrix(uint8 index, Matrix value) { matParam[index] = value; }

	array<int32, MATERIAL_ARG_COUNT>		intParam;
	array<float, MATERIAL_ARG_COUNT>		floatParam;
	array<int32, MATERIAL_ARG_COUNT>		texOnParam;
	array<Vec2, MATERIAL_ARG_COUNT>			vec2Param;
	array<Vec4, MATERIAL_ARG_COUNT>			vec4Param;
	array<Matrix, MATERIAL_ARG_COUNT>		matParam;
};

/*-------------
	Material
--------------*/
class Material : public Object
{
public:
	Material();
	virtual ~Material();

public:
	/* ----- Serializer Function ------ */
	void Save(const wstring& path) override;
	void Load(const wstring& path) override;

public:
	/* ----- External Function ----- */
	void PushGraphicsData();
	void PushComputeData();

	/* ----- Helper Function ----- */
	inline Ref<class Shader> GetShader() { return m_shader; }

	void SetShader(Ref<class Shader> shader) { m_shader = shader; }
	void SetInt(uint8 index, int32 value) { m_params.SetInt(index, value); }
	void SetFloat(uint8 index, float value) { m_params.SetFloat(index, value); }
	void SetTexture(uint8 index, Ref<class Texture> texture) {
		m_textures[index] = texture;
		m_params.SetTexOn(index, (texture == nullptr) ? 0 : 1);
	}
	void SetVec2(uint8 index, Vec2 value) { m_params.SetVec2(index, value); }
	void SetVec4(uint8 index, Vec4 value) { m_params.SetVec4(index, value); }
	void SetMatrix(uint8 index, Matrix value) { m_params.SetMatrix(index, value); }


	int32 GetInt(uint8 index) { return m_params.intParam[index]; }
	Vec4 GetVec4(uint8 index) { return m_params.vec4Param[index]; }

	void Dispatch(uint32 x, uint32 y, uint32 z);
	
	Ref<class Texture> GetTexture(uint8 index) { return m_textures[index]; }
	Ref<Material> Clone();
private:
	/* ----- Material Part Variable ----- */
	PRIVATE_PROPERTY(Ref<class Shader>, shader);
	PRIVATE_PROPERTY(MaterialParam, params);
	array<Ref<class Texture>, MATERIAL_ARG_COUNT>	m_textures;
};

