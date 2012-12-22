//
//
//
#include "Utility.hs"

struct SBranchOutput
{
    float4  vPosition             : POSITION;   // .xyzw = position
    float3  vDiffuseTexCoords     : TEXCOORD0;  // .xy = diffuse tecoords, .z = lod fade hint
    float2  vNormalTexCoords      : TEXCOORD2;  // .xy = normal map texcoords (not necessarily tied to diffuse texcoords)
    float4  vNormal               : TEXCOORD4;  // .xyz = normal-mapping vector, .w = fog
    float2  vSelfShadowTexCoords  : TEXCOORD1;  // .xy = self-shadow texcoords
    float2  vDetailTexCoords      : TEXCOORD3;  // .xy = detail map texcoords
	float4	vProjPos			  : TEXCOORD5;
};

uniform float4x4 gMatWVP;
uniform float4x4 gMatRotation;
uniform float4 gTreePosition; // z - scale


SBranchOutput main(float4   vPosition   : POSITION,     // xyz = coords, w = self-shadow texcoord s
                   float4   vNormal     : TEXCOORD0,    // xyz = normal, w = self-shadow texcoord t
                   float4   vTexCoords1 : TEXCOORD1,    // xy = diffuse texcoords, zw = wind parameters
                   float4   vTexCoords2 : TEXCOORD2,    // xy = detail texcoords, zw = normal-map texcoords
                   float4   vTexCoords3 : TEXCOORD3)    // xyz = tangent (binormal is derived from normal and tangent)
{
    // this will be fed to the branch pixel shader
    SBranchOutput sOutput;

    // define attribute aliases for readability
    float2 vShadowTexCoords = float2(vPosition.w, vNormal.w);
    float2 vDiffuseTexCoords = float2(vTexCoords1.xy);
    float2 vWindParams = float2(vTexCoords1.zw);
    float2 vDetailTexCoords = float2(vTexCoords2.xy);
    float2 vNormalTexCoords = float2(vTexCoords2.zw);
    float3 vTangent = float3(vTexCoords3.xyz);

    // scale the geometry (each tree instance can be uniquely scaled)
    vPosition.xyz *= gTreePosition.w;

	vPosition.xz = mul(float4(vPosition.xyz, 1), gMatRotation).xz;
    vNormal.xz = mul(float4(vNormal.xyz, 1), gMatRotation).xz;

    // adjust position, normal, and tagent since normal mapping is enabled
    WindEffect_Normal_Tangent(vPosition.xyz, vNormal.xyz, vTangent, vWindParams);
    
    // translate tree into position (must be done after the rotation)
    vPosition.xyz += gTreePosition.xyz;

    //
    sOutput.vPosition = mul(float4(vPosition.xyz, 1.0f), gMatWVP);
    sOutput.vNormal.rgb = vNormal.xyz;

    // pass through other texcoords exactly as they were received
    sOutput.vDiffuseTexCoords.xy = vDiffuseTexCoords.xy;
    sOutput.vNormalTexCoords.xy = vNormalTexCoords;
    sOutput.vDetailTexCoords.xy = vDetailTexCoords;
    
    sOutput.vNormal.w = 1.0f;
	sOutput.vProjPos = sOutput.vPosition;

    return sOutput;
}