/////////////////////////////////////////////////////////////////////////
//
//  
//


struct VS_IN
{
    float4 pos   : POSITION;
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

uniform float4 gHeight;

VS_OUT main(VS_IN In)
{
    VS_OUT Out;
    
    float4 worldPos = In.pos;

	worldPos.y = gHeight.x;
    
    Out.pos = mul(worldPos, matWVP);
    Out.normal = float3(0, 1, 0);
    Out.worldPos = worldPos;
    Out.projPos = Out.pos;
    Out.viewPos = mul(worldPos, matWV).xyz;
    
    return Out;
}

