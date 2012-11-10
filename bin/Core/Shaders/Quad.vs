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


VS_OUT main(VS_IN In)
{
	VS_OUT Out;
	
	Out.pos = float4(In.pos.xy, 0, 1);
	Out.uv0 = In.uv0;
	
	return Out;
}