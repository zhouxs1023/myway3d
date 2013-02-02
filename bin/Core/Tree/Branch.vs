//
//
//
#include "MTree.hs"

struct VS_IN
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
};

struct VS_OUT
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD0;
	float4 NormalDepth : TEXCOORD1;
};

uniform float4x4 matWV;
uniform float4x4 matWVP;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	//float2 vWindParams = float2(In.UV.zw);

	//WindEffect_Normal(In.Position.xyz, In.Normal, vWindParams);

	Out.Position = mul(In.Position, matWVP);
	Out.NormalDepth.xyz = mul(In.Normal, (float3x3)matWV);
	Out.UV = In.UV.xy;
	Out.NormalDepth.w = Out.Position.w;

	return Out;
}