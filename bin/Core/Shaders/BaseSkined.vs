/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float4 indices		: BLENDINDICES;
	float4 weights		: BLENDWEIGHT;
	float2 tcoord		: TEXCOORD0;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
	float4 normalDepth 	: TEXCOORD3;
};

uniform float4x4 matWorld;
uniform float4x4 matWVP;
uniform float4x4 matBlend[50];

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

	float4x4 matLocal = matBlend[In.indices[0]] * In.weights[0] +
						matBlend[In.indices[1]] * In.weights[1] +
						matBlend[In.indices[2]] * In.weights[2] +
						matBlend[In.indices[3]] * In.weights[3];

	float4 localPos = mul(In.position, matLocal);
	float3 localNormal = mul(In.normal, (float3x3)matLocal);
    
    // transfrom
    Out.position = mul(localPos, matWVP);

    //copy tcoord
    Out.tcoord = In.tcoord;
    
    Out.normalDepth.xyz = normalize(mul(localNormal, float3x3(matWorld)));
    Out.normalDepth.w = Out.position.w;
    
    return Out;
}

