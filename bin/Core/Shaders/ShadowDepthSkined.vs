/////////////////////////////////////////////////////////////////////////
//
//
//

struct VS_IN
{
	float4 pos : POSITION;
	float4 indices : BLENDINDICES;
	float4 weights : BLENDWEIGHT;
	float2 uv0 : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float2 uv0 : TEXCOORD0;
	float2 depth : TEXCOORD1;
};

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
	VS_OUT Out;

	float4x4 matLocal = _getBlendMatrix(In.indices, In.weights);

	float4 localPos = mul(matLocal, In.pos);

	Out.pos = mul(localPos, matWVP); 
	Out.uv0 = In.uv0;
	Out.depth = Out.pos.zw;
	
	return Out;
}