//
//
//
#include "MTree.hs"

struct VS_IN
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float2 UV1 : TEXCOORD1;
};

struct VS_OUT
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD0;
	float4 NormalDepth : TEXCOORD1;
};

uniform float4x4 matWV;
uniform float4x4 matP;
uniform float4 gBillboardTable[200];

VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	float2 vWindParams = float2(In.UV.zw);
	float PlacementIndex = In.UV1.x;
	float ScalarValue = In.UV1.y;

	WindEffect_Normal(In.Position.xyz, In.Normal, vWindParams);

	In.Position.xyz = mul(float4(In.Position.xyz, 1), matWV);
	In.Position.xyz += gBillboardTable[PlacementIndex] * ScalarValue;

	Out.Position = mul(In.Position, matP);
	Out.NormalDepth.xyz = mul(-In.Normal, (float3x3)matWV);
	Out.UV = In.UV.xy;
	Out.NormalDepth.w = Out.Position.w;

	return Out;
}