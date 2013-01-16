/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float2 xz			: POSITION;
	float4 normal		: NORMAL;
	float  y			: TEXCOORD0;
	float delta			: BLENDWEIGHT;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord0		: TEXCOORD0;
	float4 tcoord12		: TEXCOORD1;
	float4 tcoord34		: TEXCOORD2;
	float4 normalDepth 	: TEXCOORD3;
	float3 T			: TEXCOORD4;
	float3 B			: TEXCOORD5;
	float3 N			: TEXCOORD6;
};


uniform mat4x4      matView;
uniform mat4x4      matWVP;

uniform float4		gTransform;
uniform float4		gUVParam;
uniform float4		gUVScale;
uniform float       gMorph;

float4 CalcuPosition(float2 xz, float y)
{
	float4 r = float4(xz.x, y, xz.y, 1);

	r.xz += gTransform.xz;

	return r;
}

float2 CalcuBlendUV(float2 xz) // local space
{
	float2 uv = xz * gUVParam.xy;
	
	uv.y = 1 - uv.y;
	
	return uv;
}

float2 CalcuDetailUV(float2 xz) // world space
{
	float2 uv = xz * gUVParam.zw;
	
	uv.y = 1 - uv.y;
	
	return uv;
}


VS_OUT main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	float4 position = CalcuPosition(In.xz, In.y);
	float3 normal = In.normal.xyz * 2 - 1;
	
	float2 detailUV = CalcuDetailUV(position.xz);

	// morph
    //position.y += morph * In.delta;

    Out.position = mul(position, matWVP);
    
    Out.tcoord0 = CalcuBlendUV(In.xz);
    Out.tcoord12.xy = detailUV * gUVScale.x;
    Out.tcoord12.zw = detailUV * gUVScale.y;
    Out.tcoord34.xy = detailUV * gUVScale.z;
    Out.tcoord34.zw = detailUV * gUVScale.w;
    
    Out.normalDepth.xyz = mul(normal, (float3x3)matView);
    Out.normalDepth.w = Out.position.w;

	float3 T = float3(1, 0, 0);
	float3 B = float3(0, 0, 1);
	float3 N = normal;

	B = cross(N, T);
	T = cross(B, N);

	float3x3 matTBN = float3x3(T, B, N);
	matTBN = mul(matTBN, (float3x3)matView);
	Out.T = matTBN[0];
	Out.B = matTBN[1];
	Out.N = matTBN[2];
    
    return Out;
}