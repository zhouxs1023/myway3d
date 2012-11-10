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

uniform float4 gTransform;

uniform float4x4 matView;
uniform float4x4 matProj;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;
	
	float2 v = (In.uv0 - 0.5) * float2(1, -1) * gTransform.w;
	
	In.pos.xyz += gTransform.xyz;
	In.pos = mul(In.pos, matView); 
	In.pos.xy += v;
	Out.pos = mul(In.pos, matProj);
	Out.uv0 = In.uv0;

	return Out;
}