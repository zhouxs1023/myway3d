/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position	: POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float4 position	: POSITION;
	float2 uv : TEXCOORD0;
};

uniform float4 gTransform;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    float2 xy = In.uv * gTransform.xy + gTransform.zw;
	xy = (xy * 2 - 1) * float2(1, -1);
    Out.position = float4(xy, 0, 1);
	Out.uv = In.uv;
    
    return Out;
}

