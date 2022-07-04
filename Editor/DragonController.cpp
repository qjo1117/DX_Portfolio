#include "pch.h"
#include "DragonController.h"
#include "Animator.h"


void DragonController::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::N_1)) {
		Ref<Animator> anim = GetGameObject()->GetAnimator();
		int32 count = anim->GetAnimCount();
		int32 currentCount = anim->GetCurrentClipIndex();

		int32 index = (currentCount + 1) % count;
		anim->Play(index);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::N_2)) {
		Ref<Animator> anim = GetGameObject()->GetAnimator();
		int32 count = anim->GetAnimCount();
		int32 currentCount = anim->GetCurrentClipIndex();

		int32 index = (currentCount - 1 + count) % count;
		anim->Play(index);
	}
}
