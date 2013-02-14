//
//

#define SPEEDTREE_NUM_WIND_MATRICES 4

uniform float gWindMatrixOffset;
uniform float4x4 gWindMatrices[SPEEDTREE_NUM_WIND_MATRICES];

void WindMatrixLerp(inout float3 vCoord, int nIndex, float fWeight)
{
    vCoord = lerp(vCoord, mul(float4(vCoord, 1), gWindMatrices[nIndex]).xyz, fWeight);
}

void WindEffect_Normal_Tangent(inout float3 vPosition, inout float3 vNormal, inout float3 vTangent, float2 vWindInfo)
{
    float index = vWindInfo.x;
	float weight = vWindInfo.y;

    index = fmod(index + gWindMatrixOffset.x, SPEEDTREE_NUM_WIND_MATRICES);
    
    WindMatrixLerp(vPosition, int(index), weight);
    WindMatrixLerp(vNormal, int(index), weight);
    WindMatrixLerp(vTangent, int(index), weight);
}


void WindEffect_Normal(inout float3 vPosition, inout float3 vNormal, float2 vWindInfo)
{
	float index = vWindInfo.x;
	float weight = vWindInfo.y;

    index = fmod(index + gWindMatrixOffset.x, SPEEDTREE_NUM_WIND_MATRICES);

    WindMatrixLerp(vPosition, int(index), weight);
    WindMatrixLerp(vNormal, int(index), weight);
}


void WindEffect(inout float3 vPosition, float2 vWindInfo)
{
    float index = vWindInfo.x;
	float weight = vWindInfo.y;

    index = fmod(index + gWindMatrixOffset.x, SPEEDTREE_NUM_WIND_MATRICES);
    
    WindMatrixLerp(vPosition, int(index), weight);
}