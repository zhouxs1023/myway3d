/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float2 tcoord		: TEXCOORD0;
	float4 tangent		: TEXCOORD1;
	float4 indices		: TEXCOORD2;
	float4 weights		: TEXCOORD3;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
	float4 normalDepth 	: TEXCOORD3;
};

uniform float4x4 matBlend[50];
uniform float4x4 matV;
uniform float4x4 matVP;

void Skin(inout float4 position, inout float3 normal, in float4 indices, in float4 weights)
{
	// the skinned attributes
	float4 skinnedPos    = 0;
	float3 skinnedNormal = 0;
	
	// perform the skinning
	const int numBones = 4;
	for (int i=0; i<numBones; i++)
	{
		skinnedPos    += mul(position, matBlend[ indices[i] ]) * weights[i];
		skinnedNormal += mul(normal, (float3x3)matBlend[ indices[i] ]) * weights[i];
	}
	
	// apply the result on the input variables
	position = skinnedPos;
	normal   = skinnedNormal;
}

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

	float4x4 matLocal = matBlend[In.indices[0]] * In.weights[0] +
						matBlend[In.indices[1]] * In.weights[1] +
						matBlend[In.indices[2]] * In.weights[2] +
						matBlend[In.indices[3]] * In.weights[3];

	float4 localPos = mul(In.position, matLocal);
	float3 localNormal = mul(In.normal, (float3x3)matLocal);

	Skin(localPos, localNormal, In.indices, In.weights);
    
    // transfrom
    Out.position = mul(localPos, matVP);

    //copy tcoord
    Out.tcoord = In.tcoord;
    
    Out.normalDepth.xyz = normalize(mul(localNormal, float3x3(matV)));
    Out.normalDepth.w = Out.position.w;
    
    return Out;
}

