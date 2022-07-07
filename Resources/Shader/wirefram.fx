#ifndef _WIREFRAME_FX_
#define _WIREFRAME_FX_

#include "params.fx"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;

    row_major matrix matWorld : W;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint instancingID : SV_InstanceID;
};

struct VS_OUT
{
    float4 pos : SV_Position;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
   
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color;
    
    if (g_int_0 == 0) {
        color = float4(0.2f, 1.0f, 0.3f, 1.0f);
    }
    else {
        color = float4(1.0f, 0.2f, 0.3f, 1.0f);
    }
    
    return color;
}

#endif