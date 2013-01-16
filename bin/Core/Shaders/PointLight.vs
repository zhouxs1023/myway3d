/////////////////////////////////////////////////////////////////////////
//
//
//

struct VS_IN
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float2 uv0 : TEXCOORD0;
	float4 color : TEXCOORD1;
};

uniform float4x4 matWV;
uniform float4x4 matProj;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;
	
	float4 vpos = mul(float4(0, 0, 0, 1), matWV);
	vpos.xy += In.pos.zw;
	
	Out.pos = mul(vpos, matProj);
	Out.uv0 = In.pos.xy;
	Out.color = In.color;
	
	return Out;
}