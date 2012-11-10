/////////////////////////////////////////////////////////////////////////
//
//
//

struct VS_IN
{
	float4 pos : POSITION;
	float2 a   : TEXCOORD0;
	float2 uv0 : TEXCOORD1;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float2 uv0 : TEXCOORD0;
	float4 worldPos : TEXCOORD1;
	float4 projPos : TEXCOORD2;
	float a : TEXCOORD3;
};

uniform float4x4 matView;
uniform float4x4 matProj;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;
	
	float2 v = (In.uv0 - 0.5) * float2(1, -1) * In.pos.w;
	
	float4 wPos = float4(In.pos.xyz, 1);
	
	Out.worldPos = wPos;
	
	wPos = mul(wPos, matView); 
	wPos.xy += v;
	
	Out.pos = mul(wPos, matProj);
	Out.uv0 = In.uv0;
	
	Out.projPos = Out.pos;
	Out.a = In.a;
	
	return Out;
}