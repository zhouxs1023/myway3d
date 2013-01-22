//
//
//

struct VS_IN
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct VS_OUT
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD0;
	float4 NormalDepth : TEXCOORD1;
};

uniform float4x4 matWV;
uniform float4x4 matWVP;
uniform float4 gUVScale;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	Out.Position = mul(In.Position, matWVP);
	Out.NormalDepth.xyz = mul(In.Normal, (float3x3)matWV);
	Out.UV = In.UV * gUVScale.xy;
	Out.NormalDepth.w = Out.Position.w;

	return Out;
}