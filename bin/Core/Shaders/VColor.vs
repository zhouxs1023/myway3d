/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float4 color		: COLOR;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float4 color		: TEXCOORD0;
};

uniform mat4x4 matWVP;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    Out.position = mul(In.position, matWVP);
	Out.color = In.color;
    
    return Out;
}

