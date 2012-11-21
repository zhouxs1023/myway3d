/////////////////////////////////////////////////////////////////////////
//
//
//

struct VS_IN
{
	float4 pos : POSITION;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float2 uv0 : TEXCOORD0;
};

uniform float4 gU; // x: u, y: voff, z: 1 / (1 + voff)

uniform float4x4 matWVP;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	float u = gU.x;
	float v = 1 - saturate((In.pos.y + gU.y) * gU.z);

	Out.uv0 = float2(u, v);

	Out.pos = mul(In.pos, matWVP);

	return Out;
}