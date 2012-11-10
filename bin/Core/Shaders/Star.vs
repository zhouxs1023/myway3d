/////////////////////////////////////////////////////////////////////////
//
//
//

struct VS_IN
{
	float4 pos : POSITION;
	float3 uv0 : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float4 clr : COLOR;
	float2 uv0 : TEXCOORD0;
};

uniform float4 gTransform;
uniform float4 gSizeParam;
uniform float gAspect;

uniform float4x4 matWVP;


VS_OUT main(VS_IN In)
{
	VS_OUT Out;
	
	float mag_scale = gSizeParam.x;
	float mag0_size = gSizeParam.y;
	float min_size = gSizeParam.z;
	float max_size = gSizeParam.w;

    float4 in_color = float4(1, 1, 1, 1);
    
    In.pos.xyz = In.pos.xyz * gTransform.w + gTransform.xyz;
    Out.pos = mul(In.pos, matWVP);
    Out.uv0 = In.uv0.xy;
    
    float magnitude = In.uv0.z;
    float size = exp(mag_scale * magnitude) * mag0_size;

    // Fade below minSize.
    float fade = saturate(size / min_size);
    Out.clr = float4(in_color.rgb, fade * fade);

    // clamp size to range.
    size = clamp(size, min_size, max_size);

    // Splat the billboard on the screen.
    Out.pos.xy += In.pos.w * In.uv0.xy * float2(size, size * gAspect);
            
	return Out;
}