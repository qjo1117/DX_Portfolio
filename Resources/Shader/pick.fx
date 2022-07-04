#ifndef _PICK_FX_
#define _PICK_FX_

#include "params.fx"

struct VS_IN
{
    float3 pos : POSITION;

    row_major matrix matWorld : W;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint instancingID : SV_InstanceID;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 clipPos : POSITION;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.f;

    if (g_int_3 == 1)
    {
        output.pos = mul(float4(input.pos, 1.f), input.matWVP);
    }
    else
    {
        output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    }

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = g_vec4_0;
    clamp(color, float4(0.0f, 0.0f, 0.0f, 0.0f), float4(1.0f, 1.0f, 1.0f, 1.0f));
    return float4(color);
}

#endif