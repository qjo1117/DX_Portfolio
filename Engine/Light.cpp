#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Engine.h"
#include "Resources.h"
#include "Camera.h"
#include "Transform.h"
#include "Texture.h"
#include "SceneManager.h"


Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
	m_shadowCamera = make_shared<GameObject>();
	m_shadowCamera->AddComponent(make_shared<Transform>());
	m_shadowCamera->AddComponent(make_shared<Camera>());
	m_shadowCamera->GetCamera()->SetCullingMaskLayerOnOff(static_cast<uint32>(LAYER_TYPE::UI), true);
/*	_shadowCamera->GetTransform()->SetParent(GetGameObject()->GetTransform());	*/	// 개인적인 추가
	m_shadowCamera->name = L"ShadowCamera";

	m_shadowTex = GET_SINGLE(Resources)->Get<Texture>(L"ShadowTarget");
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	Vec3 pos = GetTransform()->GetWorldPosition();
	m_lightInfo.position = Vec4(pos.x, pos.y, pos.z, 1.0f);
	
	m_shadowCamera->GetTransform()->localPosition = GetTransform()->localPosition;
	m_shadowCamera->GetTransform()->localRotation = GetTransform()->localRotation;
	m_shadowCamera->GetTransform()->localScale = GetTransform()->localScale;

	m_shadowCamera->FinalUpdate();
}

void Light::EditorUpdate()
{
	if (ImGui::CollapsingHeader("Light")) {
		static float floatTemp[3] = {};
		Vec4 vecTemp;

		/* ----- BaseLight ----- */
		if (ImGui::TreeNode("BaseLight")) {

			ImGui::ColorEdit3("Diffuse",	(float*)&m_lightInfo.color.diffuse);
			ImGui::ColorEdit3("Ambient",	(float*)&m_lightInfo.color.ambient);
			ImGui::ColorEdit3("Specular",	(float*)&m_lightInfo.color.specular);

			ImGui::TreePop();
		}

		ImGui::Separator();

		/* ----- LightInfo ----- */
		if (ImGui::TreeNode("LightInfo")) {
			const float dragSpeed = (ImGui::GetIO().KeyShift) ? 0.5f : 0.05f;
			if (ImGui::DragFloat3("DirectionLight", (float*)&m_lightInfo.direction, dragSpeed, -2 * XM_PI, 2 * XM_PI)) {
				
			}

			if (m_lightInfo.lightType == static_cast<int32>(LIGHT_TYPE::DIRECTIONAL_LIGHT)) {

			}
			else if (m_lightInfo.lightType == static_cast<int32>(LIGHT_TYPE::POINT_LIGHT)) {
				ImGui::DragFloat("Range", &m_lightInfo.range, dragSpeed, 0.0f, 10000.0f);
			}
			else if (m_lightInfo.lightType == static_cast<int32>(LIGHT_TYPE::SPOT_LIGHT)) {
				ImGui::DragFloat("Angle", &m_lightInfo.angle, XM_PI / 360, 0.0f, XM_PI * 2);
				ImGui::DragFloat("Range", &m_lightInfo.range, dragSpeed, 0.0f, 10000.0f);
			}

			ImGui::TreePop();
		}
	}

}

void Light::Render()
{
	assert(m_lightIndex >= 0);

	GetTransform()->PushData();

	switch (static_cast<LIGHT_TYPE>(m_lightInfo.lightType)) {
	case LIGHT_TYPE::DIRECTIONAL_LIGHT: {
		m_lightMaterial->SetTexture(2, m_shadowTex);

		Matrix matVP = m_shadowCamera->GetCamera()->matView * m_shadowCamera->GetCamera()->matProjection;
		m_lightMaterial->SetMatrix(0, matVP);
		break;
	}
	case LIGHT_TYPE::POINT_LIGHT:
	case LIGHT_TYPE::SPOT_LIGHT:
		float scale = 2 * m_lightInfo.range;
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
		break;
	}

	m_lightMaterial->SetInt(0, m_lightIndex);		// Shadar에서 넘겨주는 거 생각하면 됨
	m_lightMaterial->PushGraphicsData();

	m_volumeMesh->Render();
}

void Light::RenderShadow()
{
	m_shadowCamera->GetCamera()->SortShadowGameObject();
	m_shadowCamera->GetCamera()->Render_Shadow();
}

void Light::SetLightDirection(Vec3 direction)
{
	direction.Normalize();
	m_lightInfo.direction = direction;
	GetTransform()->LookAt(direction);
}

void Light::SetLightType(LIGHT_TYPE type)
{
	m_lightInfo.lightType = static_cast<int32>(type);

	switch (type) {
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		m_volumeMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		m_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"DirLight");

		m_shadowCamera->GetCamera()->scale = 1.0f;
		m_shadowCamera->GetCamera()->Far = 10000.f;
		m_shadowCamera->GetCamera()->width = 4096;
		m_shadowCamera->GetCamera()->height = 4096;
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		m_volumeMesh = GET_SINGLE(Resources)->LoadSphereMesh();
		m_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		m_volumeMesh = GET_SINGLE(Resources)->LoadConeMesh();
		m_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	}

}
