/////////////////////////////////////////////////////////////////////////
//
//
//


struct VS_IN
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
	float4 tm			: TEXCOORD3;
};

struct VS_OUT
{
	float4 position		: POSITION;
	float2 tcoord		: TEXCOORD0;
	float4 normalDepth 	: TEXCOORD3;
};

uniform float4 normal;
uniform mat4x4 matWVP;
uniform float Time;

VS_OUT main(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;
	
	//
	float3 Origin = In.tm.xyz;

	float2 inc = float2(
					(cos(Time * 0.53f + Origin.z * 0.1f + Origin.x * 0.1f) + sin(Time * 0.53f + Origin.z * 0.4f + Origin.x * 0.67f) * 0.2f) * 0.1f,
					(sin(Time * 0.7f + Origin.x * 0.13f + Origin.z * 0.13f) + cos(Time * 0.7f + Origin.z * 0.57f + Origin.x * 0.28f) * 0.2f) * 0.1f
					);
	if (In.tcoord.y < 0.5f)
	{
		In.position.xz += inc;
		In.position.y -= length(inc) * 0.5f;
	}	
	
    // transfrom
	In.position.xyz = In.position.xyz * In.tm.w + In.tm.xyz;
	In.position.w = 1;

    Out.position = mul(In.position, matWVP);

    //copy tcoord
    Out.tcoord = In.tcoord;
    
    Out.normalDepth.xyz = normal.xyz;
    Out.normalDepth.w = Out.position.w;
    
    return Out;
}

