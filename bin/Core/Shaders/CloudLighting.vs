/////////////////////////////////////////////////////////////////////////
//
//
//

struct VS_IN
{
	float4 pos : POSITION;
	float2 uv0 : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float2 uv0 : TEXCOORD0;
};

uniform float4 gTranslate;
uniform float4 gScale;

uniform float4x4 matWVP;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;
	
	In.pos.xyz = In.pos.xyz * gScale.xyz + gTranslate.xyz;
	
	Out.pos = mul(In.pos, matWVP);
	Out.uv0 = In.uv0;
	
	return Out;
};