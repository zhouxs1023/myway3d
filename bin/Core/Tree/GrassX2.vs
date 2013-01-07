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
	float4 normalDepth 	: TEXCOORD3;
};

uniform float4 normal;
uniform mat4x4 matWVP;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
    // transfrom
    Out.position = mul(In.position, matWVP);

    //copy tcoord
    Out.tcoord = In.tcoord;
    
    Out.normalDepth.xyz = normal.xyz;
    Out.normalDepth.w = Out.position.w;
    
    return Out;
}

