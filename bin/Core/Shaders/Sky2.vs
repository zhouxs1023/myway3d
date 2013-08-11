/////////////////////////////////////////////////////////////////////////
//
//
//

float scale(float cos, float uScaleDepth)
{
	float x = 1.0 - cos;
	return uScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

struct VS_IN
{
	float4 iPosition	        : POSITION;
};

struct VS_OUT
{
	float4 oPosition		: POSITION;
	float2 oUV              : TEXCOORD0;
	float3 oRayleighColor   : TEXCOORD1;
	float3 oMieColor        : TEXCOORD2;
	float3 oDirection       : TEXCOORD3;
	float  oOpacity         : TEXCOORD4;
	float  oHeight          : TEXCOORD5;
};

uniform float4x4 uWorldViewProj;
uniform float3 uLightDir;
uniform float3 uCameraPos;

uniform float3 uInvWaveLength;

uniform float  uInnerRadius;

// Scattering parameters
uniform float  uKrESun; // Kr * ESun
uniform float  uKmESun; // Km * ESun
uniform float  uKr4PI;  // Kr * 4 * PI
uniform float  uKm4PI;  // Km * 4 * PI

// Atmosphere properties
uniform float uScale;               // 1 / (outerRadius - innerRadius)
uniform float uScaleDepth;          // Where the average atmosphere density is found
uniform float uScaleOverScaleDepth; // Scale / ScaleDepth

VS_OUT main(VS_IN In)
{
	VS_OUT Out;

    // Clip space position
	Out.oPosition = mul(In.iPosition, uWorldViewProj);

	float3 NPos = In.iPosition.xyz;

	NPos.y = max(0, NPos.y);
	NPos = normalize(NPos);

	float3 v3Pos = NPos;
	v3Pos.y += uInnerRadius;

    float3 v3Ray = v3Pos - uCameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	
	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = uCameraPos;
	float fHeight = uCameraPos.y;
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;

	// NOTE: fDepth is not pased as parameter(like a constant) to avoid the little precission issue (Apreciable)
	float fDepth = exp(uScaleOverScaleDepth * (uInnerRadius - uCameraPos.y));
	float fStartOffset = fDepth * scale(fStartAngle, uScaleDepth);

    // Init loop variables
	float fSampleLength = fFar / 4;
	float fScaledLength = fSampleLength * uScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5f;
	
	// Loop the ray
	float3 color = 0;

	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(uScaleOverScaleDepth * (uInnerRadius-fHeight));
		
		float fLightAngle = dot(uLightDir, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle, uScaleDepth) - scale(fCameraAngle, uScaleDepth)));
		float3 v3Attenuate = exp(-fScatter * (uInvWaveLength * uKr4PI + uKm4PI)); // <<< TODO
		
		// Accumulate color
		v3Attenuate *= (fDepth * fScaledLength);
		color += v3Attenuate;
		
		// Next sample point
		v3SamplePoint += v3SampleRay;
	}

	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(uScaleOverScaleDepth * (uInnerRadius-fHeight));
		
		float fLightAngle = dot(uLightDir, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle, uScaleDepth) - scale(fCameraAngle, uScaleDepth)));
		float3 v3Attenuate = exp(-fScatter * (uInvWaveLength * uKr4PI + uKm4PI)); // <<< TODO
		
		// Accumulate color
		v3Attenuate *= (fDepth * fScaledLength);
		color += v3Attenuate;
		
		// Next sample point
		v3SamplePoint += v3SampleRay;
	}

	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(uScaleOverScaleDepth * (uInnerRadius-fHeight));
		
		float fLightAngle = dot(uLightDir, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle, uScaleDepth) - scale(fCameraAngle, uScaleDepth)));
		float3 v3Attenuate = exp(-fScatter * (uInvWaveLength * uKr4PI + uKm4PI)); // <<< TODO
		
		// Accumulate color
		v3Attenuate *= (fDepth * fScaledLength);
		color += v3Attenuate;
		
		// Next sample point
		v3SamplePoint += v3SampleRay;
	}

	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(uScaleOverScaleDepth * (uInnerRadius-fHeight));
		
		float fLightAngle = dot(uLightDir, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle, uScaleDepth) - scale(fCameraAngle, uScaleDepth)));
		float3 v3Attenuate = exp(-fScatter * (uInvWaveLength * uKr4PI + uKm4PI)); // <<< TODO
		
		// Accumulate color
		v3Attenuate *= (fDepth * fScaledLength);
		color += v3Attenuate;
		
		// Next sample point
		v3SamplePoint += v3SampleRay;
	}

    // Outputs
    Out.oRayleighColor = color * (uInvWaveLength * uKrESun); // TODO <--- parameter
    Out.oMieColor = color * uKmESun; // TODO <--- *uInvMieWaveLength
    Out.oDirection = uCameraPos - v3Pos;
    Out.oHeight = 1-In.iPosition.y;

	return Out;
}
