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

uniform float Time;
uniform mat4x4 matW;
uniform mat4x4 matWV;
uniform mat4x4 matVP;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
    
    // transfrom
	float t = pow(In.position.y / 1500.0f, 2);
	float4x4 matrix2 = matW;
	matrix2._41 += 12 * sin(Time * 0.35f + matW._43 / 20.0f);
	matrix2._43 += 12 * cos(Time * 0.35f + matW._41 / 20.0f);
	float4 inPos2 = mul(In.position, matrix2);
	float4 inPos = mul(In.position, matW);
	inPos = lerp(inPos, inPos2, t);

    Out.position = mul(inPos, matVP);

    //copy tcoord
    Out.tcoord = In.tcoord;
    
    Out.normalDepth.xyz = normalize(mul(In.normal, float3x3(matWV)));
    Out.normalDepth.w = Out.position.w;
    
    return Out;
}

