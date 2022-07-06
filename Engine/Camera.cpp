#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Engine.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ParticleSystem.h"
#include "InstancingManager.h"
#include "SceneManager.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
	_vecDeferred.reserve(1000);
	_vecForward.reserve(1000);
	_vecParticle.reserve(1000);
	_vecShadow.reserve(1000);
	_vecPick.reserve(1000);

	m_width = static_cast<float>(GEngine->GetWindow().width);
	m_height = static_cast<float>(GEngine->GetWindow().height);

	_PickTargetView = GET_SINGLE(Resources)->Get<Texture>(L"PickTargetView");
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	// 상세설명 : view는 꺼꾸로 봐야지 맞다.
	m_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	if (m_CameraType == PROJECTION_TYPE::PERSPECTIVE) {
		m_matProjection = ::XMMatrixPerspectiveFovLH(m_fov, m_width / m_height, m_Near, m_Far);
	}
	else {
		m_matProjection = ::XMMatrixOrthographicLH(m_width * m_scale, m_height * m_scale, m_Near, m_Far);
	}

	m_frustum.FinalUpdate();
}

void Camera::EditorUpdate()
{
	if (ImGui::CollapsingHeader("Camera")) {
		// TODO : 직교랑 절두랑 분기를 시켜줘야한다.
		ImGui::SliderFloat("Near", &Near, 0.001f, Far - 1);
		ImGui::SliderFloat("Far", &Far, Near + 1, 10000.0f);
		ImGui::SliderFloat("Fov", &fov, 0.0f, XM_PI);
	}
}

void Camera::SortGameObject()
{
	Ref<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	const vector<Ref<GameObject>>& gameObjects = scene->GetRender();

	_vecForward.clear();
	_vecDeferred.clear();
	_vecParticle.clear();

	S_MatView = m_matView;
	S_MatProjection = m_matProjection;

	Vec3 cameraPos = GetTransform()->GetWorldPosition();

	for (const Ref<GameObject>& go : gameObjects) {
		if (IsCulled(go->GetLayer())) {
			continue;
		}

		if (go->GetCheckFrustum()) {
			if (m_frustum.ContainSphere(
				go->GetTransform()->GetWorldPosition(),
				go->GetTransform()->GetBoundingSphereRadius()) == false) {
				continue;
			}
		}

		// MeshRenderer가 있을경우에만 실행을 해준다.
		if (go->GetMeshRenderer()) {
			SHADER_TYPE shaderType = go->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
			switch (shaderType) {
			case SHADER_TYPE::DEFERRED:
				_vecDeferred.push_back(go);
				break;
			case SHADER_TYPE::FORWARD:
				_vecForward.push_back(go);
				break;
			}
		}
		else {
			_vecParticle.push_back(go);
		}
	}

}

void Camera::SortForwadObject()
{
	Ref<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	const vector<Ref<GameObject>>& gameObjects = scene->GetRender();

	_vecForward.clear();
	_vecParticle.clear();

	S_MatView = m_matView;
	S_MatProjection = m_matProjection;

	for (const Ref<GameObject>& go : gameObjects) {
		if (IsCulled(go->GetLayer())) {
			continue;
		}

		// MeshRenderer가 있을경우에만 실행을 해준다.
		if (go->GetMeshRenderer()) {
			SHADER_TYPE shaderType = go->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
			switch (shaderType) {
			case SHADER_TYPE::FORWARD:
				_vecForward.push_back(go);
				break;
			}
		}
		else {
			_vecParticle.push_back(go);
		}
	}
}

void Camera::SortShadowGameObject()
{
	Ref<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	const vector<Ref<GameObject>>& gameObjects = scene->GetShadow();

	_vecShadow.clear();

	Vec3 cameraPos = GetTransform()->GetWorldPosition();

	for (const Ref<GameObject>& go : gameObjects) {

		if (IsCulled(go->GetLayer())) {
			continue;
		}
		if (go->GetCheckFrustum()) {
			if (m_frustum.ContainSphere(
				go->GetTransform()->GetWorldPosition(),
				go->GetTransform()->GetBoundingSphereRadius()) == false) {
				continue;
			}
		}

		if (go->GetMeshRenderer() == nullptr) {
			continue;
		}

		_vecShadow.push_back(go);
	}
}

void Camera::SortPickGameObject()
{
	Ref<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	const vector<Ref<GameObject>>& gameObjects = scene->GetRender();

	S_MatView = m_matView;
	S_MatProjection = m_matProjection;

	for (const Ref<GameObject>& go : _vecShadow) {
		if (IsCulled(go->GetLayer())) {
			continue;
		}
		if (go->GetCheckFrustum()) {
			if (m_frustum.ContainSphere(
				go->GetTransform()->GetWorldPosition(),
				go->GetTransform()->GetBoundingSphereRadius()) == false) {
				continue;
			}
		}

		if (go->GetMeshRenderer() == nullptr) {
			continue;
		}
		_vecPick.push_back(go);
	}
}

void Camera::Render_Deferred()
{
	S_MatView = m_matView;
	S_MatProjection = m_matProjection;

	// 그냥 그리지말고 나눠준다.
	GET_SINGLE(InstancingManager)->Render(_vecDeferred);
}

void Camera::Render_Forward()
{
	GET_SINGLE(InstancingManager)->Render(_vecForward);

	for (const Ref<GameObject>& go : _vecParticle) {
		go->GetParticleSystem()->Render();
	}
}

void Camera::Render_Shadow()
{
	S_MatView = m_matView;
	S_MatProjection = m_matProjection;

	for (const Ref<GameObject>& go : _vecShadow) {
		go->GetMeshRenderer()->RenderShadow();
	}

}

void Camera::Render_Pick()
{
	S_MatView = m_matView;
	S_MatProjection = m_matProjection;

	for (const Ref<GameObject>& go : _vecDeferred) {
		go->GetMeshRenderer()->RenderPick(go->id);
	}

}
