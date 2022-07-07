#ifndef _COLLIDER_FX_
#define _COLLIDER_FX_

#include "params.fx"
#include "utils.fx"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float3 pos : POSITION;
    float uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    output.pos = input.pos;
    output.uv = input.uv;

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float color;
    if (g_int_0 == 0) {
        color = float4(0.2f, 1.0f, 0.3f, 1.0f);
    }
    else {
        color = float4(1.0f, 0.2f, 0.3f, 1.0f);
    }
    return color;
}

#endif