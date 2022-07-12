#ifndef _TERRAIN_FX_
#define _TERRAIN_FX_

#include "params.fx"
#include "utils.fx"


// --------------
// Vertex Shader
// --------------

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.0f;

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
    PatchTess output = (PatchTess)0.0f;

    float minDistance = g_vec2_1.x;
    float maxDistance = g_vec2_1.y;

    // 각 엣지에 중간 값을 얻는다.
    float3 edge0Pos = (input[1].pos + input[2].pos) / 2.0f;     // 1 / 2
    float3 edge1Pos = (input[2].pos + input[0].pos) / 2.0f;     // 2 / 0
    float3 edge2Pos = (input[0].pos + input[1].pos) / 2.0f;     // 0 / 1

    // Local -> World
    edge0Pos = mul(float4(edge0Pos, 1.0f), g_matWorld).xyz;
    edge1Pos = mul(float4(edge1Pos, 1.0f), g_matWorld).xyz;
    edge2Pos = mul(float4(edge2Pos, 1.0f), g_matWorld).xyz;

    // Level Of Detail
    float edge0TessLevel = CalculateTessLevel(g_vec4_0.xyz, edge0Pos, minDistance, maxDistance, 4.0f);
    float edge1TessLevel = CalculateTessLevel(g_vec4_0.xyz, edge1Pos, minDistance, maxDistance, 4.0f);
    float edge2TessLevel = CalculateTessLevel(g_vec4_0.xyz, edge2Pos, minDistance, maxDistance, 4.0f);

    output.edgeTess[0] = edge0TessLevel;
    output.edgeTess[1] = edge1TessLevel; 
    output.edgeTess[2] = edge2TessLevel;
    output.insideTess = edge2TessLevel;

    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")] 
[outputcontrolpoints(3)] 
[patchconstantfunc("ConstantHS")]

HS_OUT HS_Main(InputPatch<VS_OUT, 3> input ,int vertexIndex : SV_OutputControlPointID, int patchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT) 0.0f;

    output.pos = input[vertexIndex].pos;
    output.uv = input[vertexIndex].uv;

    return output;
}

// [Terrain Shader]
// g_int_1      : TileX
// g_int_2      : TileZ
// g_float_0    : Max Tessellation Level
// g_vec2_0     : HeightMap Resolution
// g_vec2_1     : Min/Max Tessellation Distance
// g_vec4_0     : Camera Position
// g_tex_0      : Diffuse Texture
// g_tex_1      : Normal Texture
// g_tex_2      : HeightMap Texture

// --------------
// Domain Shader
// --------------

struct DS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewBinormal : BINORMAL;
};

[domain("tri")]
DS_OUT DS_Main(const OutputPatch<HS_OUT, 3> input, float3 location: SV_DomainLocation, PatchTess patch)
{
    DS_OUT output = (DS_OUT) 0.0f;

    float3 localPos = input[0].pos * location[0] + input[1].pos * location[1] + input[2].pos * location[2];
    float2 uv = input[0].uv * location[0] + input[1].uv * location[1] + input[2].uv * location[2];

    int tileCountX = g_int_1;
    int tileCountZ = g_int_2;

    int mapWidth = g_vec2_0.x;
    int mapHeight = g_vec2_0.y;

    // 1 ~ sizeX까지의 uv지만 원래 uv는  (0,1)의 값을 가지고 있다.
    float2 fullUV = float2(uv.x / (float) tileCountX, uv.y / (float) tileCountZ);
    float height = g_tex_2.SampleLevel(g_sam_0, fullUV, 0).x;       // SampleLevel은 PixelShader에서 사용하는것이 아니면 SampleLevel를 사용해야한다.

    localPos.y = height + localPos.y;

    float2 deltaUV = float2(1.0f / mapWidth, 1.0f / mapHeight);
    float2 deltaPos = float2(tileCountX * deltaUV.x, tileCountZ * deltaUV.y);

    float upHeight = g_tex_2.SampleLevel(g_sam_0, float2(fullUV.x, fullUV.y - deltaUV.y), 0).x;
    float downHeight = g_tex_2.SampleLevel(g_sam_0, float2(fullUV.x, fullUV.y + deltaUV.y), 0).x;
    float rightHeight = g_tex_2.SampleLevel(g_sam_0, float2(fullUV.x + deltaUV.x, fullUV.y), 0).x;
    float leftHeight = g_tex_2.SampleLevel(g_sam_0, float2(fullUV.x - deltaUV.x, fullUV.y), 0).x;

    float3 localTangent = float3(localPos.x + deltaPos.x, rightHeight, localPos.z) - float3(localPos.x - deltaPos.x, leftHeight, localPos.z);
    float3 localBinormal = float3(localPos.x, upHeight, localPos.z + deltaPos.y) - float3(localPos.x, downHeight, localPos.z - deltaPos.y);

    output.pos = mul(float4(localPos, 1.0f), g_matWVP);
    output.viewPos = mul(float4(localPos, 1.0f), g_matWV).xyz;

    output.viewTangent = normalize(mul(float4(localTangent, 0.0f), g_matWV)).xyz;
    output.viewBinormal = normalize(mul(float4(localBinormal, 0.0f), g_matWV)).xyz;
    output.viewNormal = normalize(cross(output.viewBinormal, output.viewTangent));

    output.uv = uv;

    return output;
}

// --------------
// Pixel Shader
// --------------

struct PS_OUT
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
};

PS_OUT PS_Main(DS_OUT input)
{
    PS_OUT output;

    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (g_tex_on_0 == 1) {
        color = g_tex_0.Sample(g_sam_0, input.uv);
    }

    float3 viewNormal = input.viewNormal;
    if (g_tex_on_1 == 1) {
        float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.0f;

        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
    }

    output.position = float4(input.viewPos.xyz, 0.0f);
    output.normal = float4(viewNormal.xyz, 0.0f);
    output.color = color;

    return output;
}

#endif