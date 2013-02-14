/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
	float4 indices		: TEXCOORD2;
	float4 weights		: TEXCOORD3;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
	float2 Depth 	: TEXCOORD3;
};

uniform float4x4 matBlend[50];
uniform float4x4 matVP;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

	float4x4 matLocal = matBlend[In.indices[0]] * In.weights[0] +
						matBlend[In.indices[1]] * In.weights[1] +
						matBlend[In.indices[2]] * In.weights[2] +
						matBlend[In.indices[3]] * In.weights[3];

	float4 localPos = mul(In.position, matLocal);
    
    Out.position = mul(localPos, matVP);

    Out.tcoord = In.tcoord;
    
    Out.Depth = Out.position.zw;
    
    return Out;
}

