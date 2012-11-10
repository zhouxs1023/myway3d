/////////////////////////////////////////////////////////////////////////
//
//  
//


struct VS_IN
{
    float4 pos   : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUT
{
    float4 pos		: POSITION;
	float3 normal	: TEXCOORD1;
	float3 worldPos : TEXCOORD2;
    float4 projPos	: TEXCOORD3;
	float3 viewPos  : TEXCOORD4;
};

uniform float4x4 matWVP;
uniform float4x4 matWV;
uniform float4 gPosition;
uniform float4 gScale;

VS_OUT main(VS_IN In)
{
    VS_OUT Out;
    
    float4 worldPos = In.pos * gScale + gPosition;
    
    Out.pos = mul(worldPos, matWVP);
    Out.normal = In.normal;
    Out.worldPos = worldPos;
    Out.projPos = Out.pos;
    Out.viewPos = mul(worldPos, matWV).xyz;
    
    return Out;
}

