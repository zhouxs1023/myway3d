/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float2 tcoord		: TEXCOORD0;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
	float4 normalDepth 	: TEXCOORD3;
};

uniform mat4x4 matWorld;
uniform mat4x4 matWVP;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
    // transfrom
    Out.position = mul(In.position, matWVP);

    //copy tcoord
    Out.tcoord = In.tcoord;
    
    Out.normalDepth.xyz = normalize(mul(In.normal, float3x3(matWorld)));
    Out.normalDepth.w = Out.position.w;
    
    return Out;
}

