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
	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeAnimation");
	_boneFinalMatrix = make_shared<StructuredBuffer>();
}

Animator::~Animator()
{
}

void Animator::FinalUpdate()
{
	_updateTime += DELTATIME;

	const AnimClipInfo& animClip = _animClips->at(_clipIndex);
	if (_updateTime >= animClip.duration) {
		_updateTime = 0.0f;
	}

	const int32 ratio = static_cast<int32>(animClip.frameCount / animClip.duration);
	_frame = static_cast<int32>(_updateTime * ratio);
	_frame = min(_frame, animClip.frameCount - 1);
	_nextFrame = min(_frame + 1, animClip.frameCount - 1);
	_frameRatio = static_cast<float>(_frame - _frame);
}


void Animator::SetAnimClip(const vector<AnimClipInfo>* animClip)
{
	_animClips = animClip;
}

void Animator::PushData()
{
	uint32 boneCount = static_cast<uint32>(_bones->size());
	if (_boneFinalMatrix->GetElementCount() < boneCount) {
		_boneFinalMatrix->Init(sizeof(Matrix), boneCount);
	}

	/* ------ Compute Shader ------ */
	Ref<Mesh> mesh = GetGameObject()->GetMeshRenderer()->GetMesh();
	mesh->GetBoneFrameDataBuffer(_clipIndex)->PushComputeSRVData(SRV_REGISTER::t8);
	mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);

	_boneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);

	_computeMaterial->SetInt(0, boneCount);
	_computeMaterial->SetInt(1, _frame);
	_computeMaterial->SetInt(2, _nextFrame);
	_computeMaterial->SetFloat(0, _frameRatio);

	uint32 groupCount = (boneCount / 256) + 1;
	_computeMaterial->Dispatch(groupCount, 1, 1);

	_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

void Animator::Play(uint32 index)
{
	assert(index < _animClips->size());
	_clipIndex = index;
	_updateTime = 0.f;
}

