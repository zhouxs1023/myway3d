/////////////////////////////////////////////////////////////////////////
//
// Cloud Pos
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
	float2 uv1 : TEXCOORD1;
	float dist : TEXCOORD2;
	float4 projPos : TEXCOORD3;
};

uniform float4 gTranslate;
uniform float4 gScale;
uniform float4 gUVScale;
uniform float4 gUVScroll;

uniform float4x4 matWVP;

VS_OUT main(VS_IN In)
{
	VS_OUT Out;
	
	In.pos.xyz = In.pos.xyz * gScale.xyz + gTranslate.xyz;
	
	Out.pos = mul(In.pos, matWVP);
	Out.uv0 = In.uv0 * gUVScale.x + gUVScroll.xy;
	Out.uv1 = In.uv0 * gUVScale.y + gUVScroll.zw;
	Out.dist = Out.pos.w;
	Out.projPos = Out.pos;
	
	return Out;
};