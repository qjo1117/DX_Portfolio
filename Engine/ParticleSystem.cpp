#include "pch.h"
#include "ParticleSystem.h"
#include "StructuredBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "Resources.h"
#include "Timer.h"
#include "Transform.h"
#include "PathManager.h"
#include "Texture.h"

#include "EditorManager.h"


ParticleSystem::ParticleSystem() : Component(COMPONENT_TYPE::PARTICLE_SYSTEM)
{
	_particleBuffer = make_shared<StructuredBuffer>();
	_particleBuffer->Init(sizeof(ParticleInfo), m_maxParticle);

	_computeSharedBuffer = make_shared<StructuredBuffer>();
	_computeSharedBuffer->Init(sizeof(ComputeShaderdInfo), 1);
	
	_mesh = GET_SINGLE(Resources)->LoadPointMesh();
	_material = GET_SINGLE(Resources)->Get<Material>(L"Particle");
	Ref<Texture> tex = GET_SINGLE(Resources)->Load<Texture>(L"Star", GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH_KEY) + L"Particle\\Star.png");
	_material->SetTexture(0, tex);

	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeParticle");
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::FinalUpdate()
{
	m_accTime += DELTATIME;

	int32 add = 0;
	if (m_createInterval < m_accTime) {
		m_accTime = m_accTime - m_createInterval;
		add = 1;
	}

	_particleBuffer->PushComputeUAVData(UAV_REGISTER::u0);
	_computeSharedBuffer->PushComputeUAVData(UAV_REGISTER::u1);

	_computeMaterial->SetInt(0, m_maxParticle);
	_computeMaterial->SetInt(1, add);

	_computeMaterial->SetVec2(1, Vec2(DELTATIME, m_accTime));
	_computeMaterial->SetVec4(0, Vec4(m_minLifeTime, m_maxLifeTime, m_minSpeed, m_maxSpeed));

	_computeMaterial->Dispatch(1, 1, 1);
}

void ParticleSystem::EditorUpdate()
{
	if (ImGui::CollapsingHeader("ParticleSystem")) {
		ImGui::DragInt("ParticleCount", (int*)&m_maxParticle, 1, 0, 10000);
		ImGui::DragFloat2("ParticleLifeTime", &m_minLifeTime, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat2("ParticleSpeed", &m_minSpeed, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat2("ParticleScale", &m_startScale, 0.1f, 0.0f, 1000.0f);

		Ref<Texture> tex = _material->GetTexture(0);;
		ImGui::Image(tex, ImVec2(50.0f, 50.0f));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_DRAG")) {
				_material->SetTexture(0, static_pointer_cast<Texture>(EDITOR->PickObject));
				EDITOR->PickObject = nullptr;
			}
			ImGui::EndDragDropTarget();
		}
	}
}

void ParticleSystem::Render()
{
	GetTransform()->PushData();

	_particleBuffer->PushGraphicsData(SRV_REGISTER::t9);
	_material->SetFloat(0, m_startScale);
	_material->SetFloat(1, m_endScale);
	_material->PushGraphicsData();

	_mesh->Render(m_maxParticle);
}

Ref<class Texture> ParticleSystem::GetTexture()
{
	return _material->GetTexture(0);
}

void ParticleSystem::SetTexture(Ref<class Texture> tex)
{
	_material->SetTexture(0, tex);
}