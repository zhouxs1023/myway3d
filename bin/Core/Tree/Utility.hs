//
//

#define SPEEDTREE_NUM_WIND_MATRICES 6

uniform float gWindMatrixOffset;
uniform float4x4 gWindMatrices[SPEEDTREE_NUM_WIND_MATRICES];

void WindMatrixLerp(inout float3 vCoord, int nIndex, float fWeight)
{
    vCoord = lerp(vCoord, mul(float4(vCoord, 1), gWindMatrices[nIndex]).xyz, fWeight);
}


void WindEffect_Normal_Tangent(inout float3 vPosition, inout float3 vNormal, inout float3 vTangent, float2 vWindInfo)
{
    // extract the wind indices & weights:
    //   vWeights.x = [0.0, 0.98] wind weight for wind matrix 1
    //   vWeights.y = [0.0, 0.98] wind weight for wind matrix 2
    //   vIndices.x = integer index for wind matrix 1
    //   vIndices.y = integer index for wind matrix 2
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    // this one instruction helps keep two instances of the same base tree from being in
    // sync in their wind behavior; each instance has a unique matrix offset 
    // (gWindMatrixOffset.x) which helps keep them from using the same set of 
    // matrices for wind
    vIndices = fmod(vIndices + gWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    // first-level wind effect - interpolate between static position and fully-blown
    // wind position by the wind weight value
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vNormal, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vTangent, int(vIndices.x), vWeights.x);
    
    // second-level wind effect - interpolate between first-level wind position and 
    // the fully-blown wind position by the second wind weight value
    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vNormal, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vTangent, int(vIndices.y), vWeights.y);
}


void WindEffect_Normal(inout float3 vPosition, inout float3 vNormal, float2 vWindInfo)
{
    // extract the wind indices & weights:
    //   vWeights.x = [0.0, 0.98] wind weight for wind matrix 1
    //   vWeights.y = [0.0, 0.98] wind weight for wind matrix 2
    //   vIndices.x = integer index for wind matrix 1
    //   vIndices.y = integer index for wind matrix 2
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    // this one instruction helps keep two instances of the same base tree from being in
    // sync in their wind behavior; each instance has a unique matrix offset 
    // (gWindMatrixOffset.x) which helps keep them from using the same set of 
    // matrices for wind
    vIndices = fmod(vIndices + gWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    // first-level wind effect - interpolate between static position and fully-blown
    // wind position by the wind weight value
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vNormal, int(vIndices.x), vWeights.x);
    
    // second-level wind effect - interpolate between first-level wind position and 
    // the fully-blown wind position by the second wind weight value
    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vNormal, int(vIndices.y), vWeights.y);
}


void WindEffect(inout float3 vPosition, float2 vWindInfo)
{
    // extract the wind indices & weights:
    //   vWeights.x = [0.0, 0.98] wind weight for wind matrix 1
    //   vWeights.y = [0.0, 0.98] wind weight for wind matrix 2
    //   vIndices.x = integer index for wind matrix 1
    //   vIndices.y = integer index for wind matrix 2
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    // this one instruction helps keep two instances of the same base tree from being in
    // sync in their wind behavior; each instance has a unique matrix offset 
    // (gWindMatrixOffset.x) which helps keep them from using the same set of 
    // matrices for wind
    vIndices = fmod(vIndices + gWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    // first-level wind effect - interpolate between static position and fully-blown
    // wind position by the wind weight value
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);
    
    // second-level wind effect - interpolate between first-level wind position and 
    // the fully-blown wind position by the second wind weight value
    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
}