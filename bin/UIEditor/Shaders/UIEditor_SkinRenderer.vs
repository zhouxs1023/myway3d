/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position	: POSITION;
};

struct VS_OUT
{
	float4 position	: POSITION;
	float2 bkuv		: TEXCOORD0;
	float2 uv		: TEXCOORD1;
};

uniform float4 gScale;
uniform float4 gBackUVScale;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

	float2 uv = In.position.xy;
	float2 pos = In.position.xy * gScale.xy;

	pos.x = (pos.x - 0.5) * 2;
	pos.y = (-pos.y + 0.5) * 2;

    Out.position = float4(pos, 0, 1);
    Out.bkuv = uv * gBackUVScale;
    Out.uv = uv;
    
    return Out;
}

