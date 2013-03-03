/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
};


VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    Out.position = In.position;
    Out.color = In.color;
    Out.uv = In.uv;
    
    return Out;
}

