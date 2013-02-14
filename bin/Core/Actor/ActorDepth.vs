/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
	float2 Depth 		: TEXCOORD3;
};

uniform mat4x4 matWVP;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
    Out.position = mul(In.position, matWVP);

    Out.tcoord = In.tcoord;
    
    Out.Depth = Out.position.zw;
    
    return Out;
}

