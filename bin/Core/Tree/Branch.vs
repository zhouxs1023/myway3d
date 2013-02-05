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

uniform float4 gTranslateScale;
uniform float4x4 gRotationMatrix;
uniform float4x4 matView;
uniform float4x4 matVP;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	float2 vWindParams = float2(In.UV.zw);

	In.Position = mul(In.Position, gRotationMatrix);
	In.Normal = mul(In.Normal, (float3x3)gRotationMatrix);

	WindEffect_Normal(In.Position.xyz, In.Normal, vWindParams);

	In.Position.xyz = In.Position.xyz * gTranslateScale.w + gTranslateScale.xyz;

	Out.Position = mul(In.Position, matVP);
	Out.NormalDepth.xyz = mul(-In.Normal, (float3x3)matView);
	Out.UV = In.UV.xy;
	Out.NormalDepth.w = Out.Position.w;

	return Out;
}