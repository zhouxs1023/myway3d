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
	float2 depth : TEXCOORD1;
};

uniform float4x4 matWVP;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	Out.pos = mul(In.pos, matWVP); 
	Out.uv0 = In.uv0;
	Out.depth = Out.pos.zw;
	
	return Out;
}