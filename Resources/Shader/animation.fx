#ifndef _ANIMATION_FX_
#define _ANIMATION_FX_

#include "params.fx"
#include "utils.fx"
#include "math.fx"

struct AnimFrameParams
{
    float4 scale;
    float4 rotation;
    float4 translation;
};

StructuredBuffer<AnimFrameParams>   g_bone_frame : register(t8);
StructuredBuffer<matrix>            g_offset : register(t9);
RWStructuredBuffer<matrix>          g_final : register(u0);

// ComputeAnimation
// g_int_0 : BoneCount
// g_int_1 : CurrentFrame
// g_int_2 : NextFrame
// g_float_0 : Ratio
[numthreads(256,1,1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    if(g_int_0 <= threadIndex.x) {
        return;
    }

    int boneCount = g_int_0;
    int currentFrame = g_int_1;
    int nextFrame = g_int_2;
    float ratio = g_float_0;

    uint index = (boneCount * currentFrame) + threadIndex.x;
    uint nextIndex = (boneCount * nextFrame) + threadIndex.x;

    float4 quaternionZero = float4(0.0f, 0.0f, 0.0f, 1.0f);

    float4 scale = lerp(g_bone_frame[index].scale, g_bone_frame[nextIndex].scale, ratio);
    float4 rotation = QuaternionSlerp(g_bone_frame[index].rotation, g_bone_frame[nextIndex].rotation, ratio);
    float4 translation = lerp(g_bone_frame[index].translation, g_bone_frame[nextIndex].translation, ratio);

    matrix matBone = MatrixAffineTransformation(scale, quaternionZero, rotation, translation);

    g_final[threadIndex.x] = mul(g_offset[threadIndex.x], matBone);
}


#endif