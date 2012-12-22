///////////////////////////////////////////////////////////////////////  
//  Branch Shaders
//
//  *** INTERACTIVE DATA VISUALIZATION (IDV) CONFIDENTIAL AND PROPRIETARY INFORMATION ***
//
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Interactive Data Visualization, Inc. and
//  may not be copied, disclosed, or exploited except in accordance with 
//  the terms of that agreement.
//
//      Copyright (c) 2003-2008 IDV, Inc.
//      All rights reserved in all media.
//
//      IDV, Inc.
//      Web: http://www.idvinc.com


///////////////////////////////////////////////////////////////////////  
//  Branch-specific global variables

// vs (none)

// ps
texture     g_tBranchDiffuseMap;            // bark image, .rgb = diffuse color, .a = alpha noise map
texture     g_tBranchNormalMap;             // bark normal map, .rgb = xyz normals, .a = unused
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
texture     g_tBranchDetailMap;             // bark detail image, .rgb = color, .a = amount of detail to use
#endif


///////////////////////////////////////////////////////////////////////  
//  Texture samplers
//
//  These are specific to the branch pixel shader.  The self-shadow layer
//  is assumed to be bound to s1.

sampler2D samBranchDiffuseMap : register(s0) = sampler_state
{
    Texture = <g_tBranchDiffuseMap>;
#ifdef SPEEDTREE_DIRECTX9
    MagFilter = SPEEDTREE_FILTER;
    MipFilter = SPEEDTREE_FILTER;
    MinFilter = SPEEDTREE_FILTER;
#endif
};

sampler2D samBranchNormalMap : register(s1) = sampler_state
{
    Texture = <g_tBranchNormalMap>;
#ifdef SPEEDTREE_DIRECTX9
    MagFilter = SPEEDTREE_FILTER;
    MipFilter = SPEEDTREE_FILTER;
    MinFilter = SPEEDTREE_FILTER;
#endif
};

#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
sampler2D samBranchDetailMap : register(s3) = sampler_state
{
    Texture = <g_tBranchDetailMap>;
#ifdef SPEEDTREE_DIRECTX9
    MagFilter = Linear;
    MipFilter = Linear;
    MinFilter = Linear;
#endif
};
#endif


///////////////////////////////////////////////////////////////////////  
//  Branch VS Ouput, PS Input
//
//  Cannot use more than four texture layers in order to compile down to ps_1_1

struct SBranchOutput
{
    float4  vPosition             : POSITION;   // .xyzw = position
#ifdef SPEEDTREE_BRANCH_FADING
    float3  vDiffuseTexCoords     : TEXCOORD0;  // .xy = diffuse tecoords, .z = lod fade hint
#else
    float2  vDiffuseTexCoords     : TEXCOORD0;  // .xy = diffuse tecoords
#endif
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
    float2  vNormalTexCoords      : TEXCOORD2;  // .xy = normal map texcoords (not necessarily tied to diffuse texcoords)
    float4  vNormal               : TEXCOORD4;  // .xyz = normal-mapping vector, .w = fog
#else
    float4  vNormal               : COLOR;      // color computed by standard lighting
#endif
#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    float2  vSelfShadowTexCoords  : TEXCOORD1;  // .xy = self-shadow texcoords
#endif
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
    float2  vDetailTexCoords      : TEXCOORD3;  // .xy = detail map texcoords
#endif
};


///////////////////////////////////////////////////////////////////////  
//  BranchVS
//
//  Branch geometry vertex shader

SBranchOutput BranchVS(float4   vPosition   : POSITION,     // xyz = coords, w = self-shadow texcoord s
                       float4   vNormal     : TEXCOORD0,    // xyz = normal, w = self-shadow texcoord t
                       float4   vTexCoords1 : TEXCOORD1,    // xy = diffuse texcoords, zw = wind parameters
                       float4   vTexCoords2 : TEXCOORD2,    // xy = detail texcoords, zw = normal-map texcoords
#ifdef SPEEDTREE_BRANCH_FADING
                       float4   vTexCoords3 : TEXCOORD3)    // xyz = tangent (binormal is derived from normal and tangent), w = lod fade hint
#else                    
                       float3   vTexCoords3 : TEXCOORD3)    // xyz = tangent (binormal is derived from normal and tangent)
#endif
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
    vPosition.xyz *= g_vTreePos.w;

    // rotate the whole tree (each tree instance can be uniquely rotated) - use optimized z-axis rotation
    // algorithm where float(sin(a), cos(a), -sin(a), 0.0f) is uploaded instead of angle a
#ifdef SPEEDTREE_UPVECTOR_Y
    vPosition.xz = float2(dot(g_vTreeRotationTrig.ywz, vPosition.xyz), dot(g_vTreeRotationTrig.xwy, vPosition.xyz));
    vNormal.xz = float2(dot(g_vTreeRotationTrig.ywz, vNormal.xyz), dot(g_vTreeRotationTrig.xwy, vNormal.xyz));
#else
    vPosition.xy = float2(dot(g_vTreeRotationTrig.yxw, vPosition.xyz), dot(g_vTreeRotationTrig.zyw, vPosition.xyz));
    vNormal.xy = float2(dot(g_vTreeRotationTrig.yxw, vNormal.xyz), dot(g_vTreeRotationTrig.zyw, vNormal.xyz));
#endif
    
    // the wind effect primarily adjusts only the position of the vertex, but it can also be used to
    // affect the lighting vectors.  #define SPEEDTREE_ACCURATE_WIND_LIGHTING to activate it.  note
    // that the binormal is later derived (and thus not adjusted here) from the normal and tangent if used
#ifdef SPEEDTREE_ACCURATE_WIND_LIGHTING
    #ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
        // adjust position, normal, and tagent since normal mapping is enabled
        WindEffect_Normal_Tangent(vPosition.xyz, vNormal.xyz, vTangent, vWindParams);
    #else
        // adjust position and normal
        WindEffect_Normal(vPosition.xyz, vNormal.xyz, vWindParams);
    #endif
#else
    // adjust only the position
    WindEffect(vPosition.xyz, vWindParams);
#endif
    
    // translate tree into position (must be done after the rotation)
    vPosition.xyz += g_vTreePos.xyz;

#ifdef SPEEDTREE_WORLD_TRANSLATE
    vPosition.xyz -= g_vCameraPos.xyz;
#endif

#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
    // setup per-pixel normal mapping  (assumes normalized light direction)
    
    // rotate the tangent by this instance's rotation and derive the binormal from the normal & tangent 
    // in order to keep vertex attribute count lower
#ifdef SPEEDTREE_UPVECTOR_Y
    vTangent.xz = float2(dot(g_vTreeRotationTrig.ywz, vTangent.xyz), dot(g_vTreeRotationTrig.xwy, vTangent.xyz));
#else
    vTangent.xy = float2(dot(g_vTreeRotationTrig.yxw, vTangent.xyz), dot(g_vTreeRotationTrig.zyw, vTangent.xyz));
#endif
    float3 vBinormal = cross(vNormal.xyz, vTangent);
    
    // 0.5f + 0.5f * expr not used since texcoords aren't clamped to [0,1] range - this approach saves
    // several instructions
    sOutput.vNormal.x = dot(g_vLightDir.xyz, vTangent.xyz);
    sOutput.vNormal.y = dot(g_vLightDir.xyz, vBinormal.xyz);
    sOutput.vNormal.z = dot(g_vLightDir.xyz, vNormal.xyz);
#else
    // simple per-vertex lighting (nearly a dozen instructions are still generated, so normal-mapping is about
    // the same expense as far as the vertex shader goes)
    sOutput.vNormal.rgb = LightDiffuse(vPosition.xyz, vNormal.xyz, g_vLightDir.xyz, g_vLightDiffuse.xyz, g_vMaterialDiffuse.xyz);
#endif
    
#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    sOutput.vSelfShadowTexCoords = MoveSelfShadow(vShadowTexCoords);
#endif
    
    // project position to the screen
    sOutput.vPosition = mul(g_mModelViewProj, float4(vPosition.xyz, 1.0f));

    // pass through other texcoords exactly as they were received
    sOutput.vDiffuseTexCoords.xy = vDiffuseTexCoords.xy;
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
    sOutput.vNormalTexCoords.xy = vNormalTexCoords;
#endif
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
    sOutput.vDetailTexCoords.xy = vDetailTexCoords;
#endif

#ifdef SPEEDTREE_BRANCH_FADING
    // compute some fade value (0.0 = transparent, 1.0 = opaque)
    float fLodFadeHint = vTexCoords3.w;
    sOutput.vDiffuseTexCoords.z = ComputeFadeValue(fLodFadeHint);
#endif
    
#ifdef SPEEDTREE_USE_FOG 
    // calc fog (cheap in vertex shader, relatively expensive later in the pixel shader)
    sOutput.vNormal.w = FogValue(sOutput.vPosition.z);
#else
    sOutput.vNormal.w = 1.0f;
#endif

    return sOutput;
}


///////////////////////////////////////////////////////////////////////  
//  BranchPS
//
//  Branch geometry pixel shader.  This shader can process up to four texture layers:
//      - diffuse bark map
//      - normal map
//      - detail map (lerped against the diffuse map using detail's alpha layer)
//      - self-shadow map (greyscale composite map generated via SpeedTreeCAD)
//
//  * Performance Note: The default reference application shaders are designed to
//    render the SpeedTreeCAD preview with 100% accuracy. Performance can be increased,
//    especially in the pixel shaders, by reducing CAD-compliance or by making assumptions 
//    about certain variable values (e.g. diffuse scale is always 1.0f, materials are 
//    always white, etc.)

float4 BranchPS(SBranchOutput In) : COLOR
{
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
    // since the normal map normal values (normally ranged [-1,1]) are stored
    // as a color value (ranged [0,1]), they must be uncompressed.  a dot product 
    // is used to compute the diffuse lighting contribution.
    float4 texNormal = tex2D(samBranchNormalMap, In.vNormalTexCoords);
    const float3 vHalves = { 0.5f, 0.5f, 0.5f };
    float fDot = saturate(dot(2.0f * (texNormal.rgb - vHalves), In.vNormal.rgb));
#endif

    // look up the diffuse map layer
    float4 texDiffuse = tex2D(samBranchDiffuseMap, In.vDiffuseTexCoords.xy);
    
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
    // if branch detail layer is active, look it up and lerp between it
    // and the base layer.  this allows for a smooth transition between diffuse
    // and detail layers if the detail layer's alpha map is done correctly
    // (the diffuse map's alpha layer doesn't figure into it)
    float4 texDetail = tex2D(samBranchDetailMap, In.vDetailTexCoords);
    texDiffuse.rgb = lerp(texDiffuse.rgb, texDetail.rgb, texDetail.a);
#endif
    
    // compute the ambient contribution (pulled from the diffuse map)
    float3 vAmbient = texDiffuse.xyz * g_vMaterialAmbient.xyz;
    
    // compute the diffuse contribution, using normal mapping or standard vertex-based lighting
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
    float3 vColor3 = texDiffuse.rgb * fDot * g_fDiffuseScale * g_vLightDiffuse.rgb;
#else
    float3 vColor3 = texDiffuse.rgb * In.vNormal.rgb * g_fDiffuseScale;
#endif

#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    // overlay the shadow if active
    float4 texShadow = tex2D(samSelfShadowMap, In.vSelfShadowTexCoords);
    vColor3 *= texShadow.xyz;
#endif    

    // compute the full lighting value
    float4 vColor = float4(vColor3 + vAmbient, texDiffuse.a);
    
#ifdef SPEEDTREE_BRANCH_FADING
    float fFade = In.vDiffuseTexCoords.z;
    vColor.a *= fFade;
#endif

#ifdef SPEEDTREE_USE_FOG
    // if fog is active, interpolate between the unfogged color and the fog color
    // based on vertex shader fog value
    vColor.xyz = lerp(g_vFogColor, vColor.xyz, In.vNormal.w).xyz;
#endif
    
    return vColor;
}


///////////////////////////////////////////////////////////////////////  
//  Techniques

technique Branches
{
    pass P0
    {          
        VS_COMPILE_COMMAND BranchVS( );
        PS_COMPILE_COMMAND BranchPS( );
    }
}


