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

uniform float4x4 matWV;
uniform float4x4 matWVP;
uniform float3x4 matBlend[75];

float4x4 _getBlendMatrix(float4 indices, float4 weights)
{
	float3x4 m = matBlend[indices[0]] * weights[0] +
				 matBlend[indices[1]] * weights[1] +
				 matBlend[indices[2]] * weights[2] +
				 matBlend[indices[3]] * weights[3];

	float4x4 rm;

	rm[0] = m[0];
	rm[1] = m[1];
	rm[2] = m[2];
	rm[3] = float4(0, 0, 0, 1);

	return rm;
}

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

	float4x4 matLocal = _getBlendMatrix(In.indices, In.weights);

	float4 localPos = mul(matLocal, In.position);

	float3 localNormal = mul((float3x3)matLocal, In.normal);
    
    // transfrom
    Out.position = mul(localPos, matWVP);

    //copy tcoord
    Out.tcoord = In.tcoord;
    
    Out.normalDepth.xyz = normalize(mul(localNormal, float3x3(matWV)));
    Out.normalDepth.w = Out.position.w;
    
    return Out;
}

