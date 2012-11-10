/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float2 tcoord		: TEXCOORD0;
	float delta		: BLENDWEIGHT;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord0		: TEXCOORD0;
	float4 tcoord12		: TEXCOORD1;
	float4 tcoord34		: TEXCOORD2;
	float4 normalDepth 	: TEXCOORD3;
};

uniform mat4x4      matWVP;
uniform float4      UVScale;
uniform float       morph;

VS_OUT main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	// morph
    //In.position.y += morph * In.delta;
    
    // transfrom
    Out.position = mul(In.position, matWVP);
    
    //copy tcoord
    Out.tcoord0 = In.tcoord;
    Out.tcoord12.xy = In.tcoord * UVScale.x;
    Out.tcoord12.zw = In.tcoord * UVScale.y;
    Out.tcoord34.xy = In.tcoord * UVScale.z;
    Out.tcoord34.zw = In.tcoord * UVScale.w;
    
    Out.normalDepth.xyz = In.normal;
    Out.normalDepth.w = Out.position.w;
    
    return Out;
}