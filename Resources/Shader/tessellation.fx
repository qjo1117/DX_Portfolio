#ifndef _TESSELLATION_FX_
#define _TESSELLATION_FX_

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

// --------------
//  Hull Shader
// --------------

struct PatchTess
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
};

struct HS_OUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

PatchTess ConstantHS(InputPatch<VS_OUT, 3> input, int patchID : SV_PrimitiveID)
{
    PatchTess output = (PatchTess) 0.0f;

    output.edgeTess[0] = g_vec4_0.x;
    output.edgeTess[1] =  g_vec4_0.y;
    output.edgeTess[2] =  g_vec4_0.z;
    output.insideTess = g_vec4_0.w;

    return output;
}

[domain("tri")]                     // 패치의 종류 (tri, quad, isoline)
[partitioning("integer")]           // subdivision mode (integer 소수점 무시, fractional_even, fractional_odd)
[outputtopology("triangle_ccw")]     // (triangle_cw, triangle_ccw, line)
[outputcontrolpoints(3)]            // 하나의 입력 패치에 대해, HS가 출력할 제어점 개수
[patchconstantfunc("ConstantHS")]   // ConstantHS 함수 이름
HS_OUT HS_Main(InputPatch<VS_OUT, 3> input, int vertexIdx : SV_OutputControlPointID, int patchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT) 0.0f;

    output.pos = input[vertexIdx].pos;
    output.uv = input[vertexIdx].uv;

    return output;
}

// ----------------
//  Domain Shader
// ----------------

struct DS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

[domain("tri")]
DS_OUT DS_Main(const OutputPatch<HS_OUT, 3> input, float3 location : SV_DomainLocation, PatchTess patch)
{
    DS_OUT output = (DS_OUT) 0.0f;

    float3 localPos = input[0].pos * location[0] + input[1].pos * location[1] + input[2].pos * location[2];
    float2 uv = input[0].uv * location[0] + input[1].uv * location[1] + input[2].uv * location[2];

    output.pos = mul(float4(localPos, 1.0f), g_matWVP);
    output.uv = uv;

    return output;
}

float4 PS_Main(DS_OUT input) : SV_Target
{
    return float4(0.2f, 1.0f, 0.3f, 1.0f);
}

#endif