#include "pch.h"

#include "Animator.h"
#include "Resources.h"
#include "Timer.h"
#include "StructuredBuffer.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Shader.h"

Animator::Animator() : Component(COMPONENT_TYPE::ANIMATOR)
{
	m_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeAnimation");
	m_boneFinalMatrix = make_shared<StructuredBuffer>();
}

Animator::~Animator()
{
}

void Animator::EditorUpdate()
{
	if (ImGui::DragInt("Clips", &m_clipIndex, 0, m_animClips->size())) {
		Play(m_clipIndex);
	}

}

void Animator::FinalUpdate()
{
	m_updateTime += DELTATIME;

	const AnimClipInfo& animClip = m_animClips->at(m_clipIndex);
	if (m_updateTime >= animClip.duration) {
		m_updateTime = 0.0f;
	}

	const int32 ratio = static_cast<int32>(animClip.frameCount / animClip.duration);
	m_frame = static_cast<int32>(m_updateTime * ratio);
	m_frame = min(m_frame, animClip.frameCount - 1);
	m_nextFrame = min(m_frame + 1, animClip.frameCount - 1);
	m_frameRatio = static_cast<float>(m_frame - m_frame);
}


void Animator::SetAnimClip(const vector<AnimClipInfo>* animClip)
{
	m_animClips = animClip;
}

void Animator::PushData()
{
	uint32 boneCount = static_cast<uint32>(m_bones->size());
	if (m_boneFinalMatrix->GetElementCount() < boneCount) {
		m_boneFinalMatrix->Init(sizeof(Matrix), boneCount);
	}

	/* ------ Compute Shader ------ */
	Ref<Mesh> mesh = GetGameObject()->GetMeshRenderer()->mesh;
	mesh->GetBoneFrameDataBuffer(m_clipIndex)->PushComputeSRVData(SRV_REGISTER::t8);
	mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);

	m_boneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);

	m_computeMaterial->SetInt(0, boneCount);
	m_computeMaterial->SetInt(1, m_frame);
	m_computeMaterial->SetInt(2, m_nextFrame);
	m_computeMaterial->SetFloat(0, m_frameRatio);

	uint32 groupCount = (boneCount / 256) + 1;
	m_computeMaterial->Dispatch(groupCount, 1, 1);

	m_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

void Animator::Play(uint32 index)
{
	assert(index < m_animClips->size());
	m_clipIndex = index;
	m_updateTime = 0.f;
}

