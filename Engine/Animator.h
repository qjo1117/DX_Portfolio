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

	virtual void EditorUpdate() override;

public:
	void SetBones(const vector<BoneInfo>* bones) { m_bones = bones; }
	void SetAnimClip(const vector<AnimClipInfo>* animClip);
	void PushData();

	int32 GetAnimCount() { return static_cast<uint32>(m_animClips->size()); }
	int32 GetCurrentClipIndex() { return m_clipIndex; }
	void Play(uint32 index);

public:
	virtual void FinalUpdate() override;

private:

	const vector<BoneInfo>*		m_bones;
	const vector<AnimClipInfo>* m_animClips;

	float						m_updateTime = 0.0f;
	int32						m_clipIndex = 0;
	int32						m_frame = 0;
	int32						m_nextFrame = 0;
	float						m_frameRatio = 0.0f;

	Ref<Material>				m_computeMaterial;
	Ref<StructuredBuffer>		m_boneFinalMatrix;
	bool						m_oneFinalUpdated = false;
};

