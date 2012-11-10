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
};

uniform float4x4 matWorld;
uniform float4x4 matWVP;


VS_OUT main(VS_IN In)
{
	VS_OUT Out;
	
	In.pos = mul(In.pos, matWorld);
	Out.pos = mul(In.pos, matWVP);
	
	return Out;
}