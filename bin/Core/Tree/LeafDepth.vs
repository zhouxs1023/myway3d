//
//
//
#include "MTree.hs"

struct VS_IN
{
	float4 Position : POSITION;
	float4 UV : TEXCOORD0;
	float2 UV1 : TEXCOORD1;
};

struct VS_OUT
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD0;
	float2 Depth : TEXCOORD1;
};

uniform float4 gTranslateScale;
uniform float4x4 gRotationMatrix;
uniform float4x4 matVP;
uniform float4 gBillboardTable[128];

VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	float2 vWindParams = float2(In.UV.zw);
	float PlacementIndex = In.UV1.x;
	float ScalarValue = In.UV1.y;

	In.Position = mul(In.Position, gRotationMatrix);

	WindEffect(In.Position.xyz, vWindParams);

	In.Position.xyz = In.Position.xyz * gTranslateScale.w + gTranslateScale.xyz;
	In.Position.xyz += gBillboardTable[PlacementIndex].xyz * ScalarValue * gTranslateScale.w;

	Out.Position = mul(In.Position, matVP);
	Out.UV = In.UV.xy;
	Out.Depth = Out.Position.zw;

	return Out;
}