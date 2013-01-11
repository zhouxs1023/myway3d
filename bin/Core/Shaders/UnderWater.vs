/////////////////////////////////////////////////////////////////////////
//
//  
//


struct VS_IN
{
    float4 pos		: POSITION;
    float3 normal	: normal;
};

struct VS_OUT
{
    float4 pos		: POSITION;
    float2 uv0		: TEXCOORD0;
	float3 normal	: TEXCOORD1;
    float4 pos_  	: TEXCOORD2;
    float4 projPos	: TEXCOORD3;
};

uniform float4x4 matWVP;
uniform float4 gPosition;
uniform float4 gScale;
uniform float4 gUVParam;

VS_OUT main(VS_IN In)
{
    VS_OUT Out;
    
    float4 worldPos = In.pos * gScale + gPosition;
    
    Out.pos_ = worldPos;
    Out.pos = mul(worldPos, matWVP);
    Out.uv0 = worldPos.xz * gUVParam.xy + gUVParam.zw;
	Out.normal = In.normal;
    Out.projPos = Out.pos;
    
    return Out;
}

