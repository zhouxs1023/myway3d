/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float2 xz			: POSITION;
	float3 normal		: NORMAL;
	float  y			: TEXCOORD0;
	float delta			: BLENDWEIGHT;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord0		: TEXCOORD0;
};


uniform mat4x4      matWVP;

uniform float4		gTransform;
uniform float       gMorph;
uniform float4		gMinInvSize;

float4 CalcuPosition(float2 xz, float y)
{
	float4 r = float4(xz.x, y, xz.y, 1);

	r.xz += gTransform.xz;

	// morph
    //position.y += gMorph * In.delta;

	return r;
}

float2 CalcuUV(float2 xz) // local space
{
	float2 uv = (xz - gMinInvSize.xy) * gMinInvSize.zw;
	
	uv.y = 1 - uv.y;
	
	return uv;
}


VS_OUT main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	float4 position = CalcuPosition(In.xz, In.y);
	
    Out.position = mul(position, matWVP);
    Out.tcoord0 = CalcuUV(In.xz);

    return Out;
}