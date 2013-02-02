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
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    vIndices = fmod(vIndices + gWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vNormal, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vTangent, int(vIndices.x), vWeights.x);
    
    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vNormal, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vTangent, int(vIndices.y), vWeights.y);
}


void WindEffect_Normal(inout float3 vPosition, inout float3 vNormal, float2 vWindInfo)
{
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    vIndices = fmod(vIndices + gWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vNormal, int(vIndices.x), vWeights.x);
    
    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vNormal, int(vIndices.y), vWeights.y);
}


void WindEffect(inout float3 vPosition, float2 vWindInfo)
{
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    vIndices = fmod(vIndices + gWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);

    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
}