#pragma once

#include "Component.h"
#include "Mesh.h"

class Material;
class StructuredBuffer;
class Mesh;

class Animator : public Component
{
public:
	Animator();
	virtual ~Animator();

public:
	void SetBones(const vector<BoneInfo>* bones) { _bones = bones; }
	void SetAnimClip(const vector<AnimClipInfo>* animClip);
	void PushData();

	int32 GetAnimCount() { return static_cast<uint32>(_animClips->size()); }
	int32 GetCurrentClipIndex() { return _clipIndex; }
	void Play(uint32 index);

public:
	virtual void FinalUpdate() override;

private:

	const vector<BoneInfo>*		_bones;
	const vector<AnimClipInfo>* _animClips;

	float						_updateTime = 0.0f;
	int32						_clipIndex = 0;
	int32						_frame = 0;
	int32						_nextFrame = 0;
	float						_frameRatio = 0.0f;

	Ref<Material>				_computeMaterial;
	Ref<StructuredBuffer>		_boneFinalMatrix;
	bool						_oneFinalUpdated = false;
};

