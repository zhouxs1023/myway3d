///////////////////////////////////////////////////////////////////////  
//  Overlay Shaders
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
//  Preprocessor

#include "CompileCommands.fx"


///////////////////////////////////////////////////////////////////////  
//  Because the PS3 does not support run-time compilation, it is necessary to
//  specify which registers the shared global variables should be stored in.  This
//  macro handles this appropriately with PS3 is #defined and is ignored otherwise.

#ifdef PS3
#define REG(x) : C##x
#else
#define REG(x)
#endif


///////////////////////////////////////////////////////////////////////  
//  Global variables

// shared by both ground and wind icon
float4x4    g_mModelViewProj REG(0);        // composite modelview/projection matrix
float3      g_vFogColor REG(4);             // color of fog (and likely the background)
float3      g_vFogParams REG(5);            // used for fog distance calcs: .x = fog start, .y = fog end, .z = end - start

// ground
float4      g_vGroundColor;                 // modulates against the ground texture             
texture     g_tGroundMap;                   // ground texture map

// wind icon
float4x4    g_mWindIconBillboardMatrix;     // special matrix used to billboard the wind icon flat against the screen
float       g_fWindIconScale;               // scale factor for the wind icon
float3      g_vWindIconPos;                 // position of the wind icon in world coordinates
float4      g_vWindIconColor;               // color for the wind icon, modulated against the wind icon
texture     g_tLocalWindEffect;             // bark image, .rgb = diffuse color, .a = alpha noise map


///////////////////////////////////////////////////////////////////////  
//  Global texture sampler

sampler2D samGroundMap : register(s0) = sampler_state
{
    Texture = <g_tGroundMap>;
#ifdef SPEEDTREE_DIRECTX9
    MagFilter = Linear;
    MipFilter = Linear;
    MinFilter = Linear;
#endif
};


///////////////////////////////////////////////////////////////////////  
//  Wind icon sampler

sampler2D samLocalWindEffect: register(s0) = sampler_state
{
    Texture = <g_tLocalWindEffect>;
#ifdef SPEEDTREE_DIRECTX9
    MagFilter = Linear;
    MipFilter = Linear;
    MinFilter = Linear;
#endif
};


///////////////////////////////////////////////////////////////////////  
//  Ground VS Ouput, PS Input

struct SGroundOutput
{
    float4 vPosition        : POSITION;     
#ifdef SPEEDTREE_USE_FOG
    float3 vTexCoords       : TEXCOORD0;    // .xy = diffuse texcoords, .z = fog
#else
    float2 vTexCoords       : TEXCOORD0;
#endif
};


///////////////////////////////////////////////////////////////////////  
//  FogValue
//
//  Simple LINEAR fog computation.  If an exponential equation is desired,
//  it can be placed here - all of the shaders call this one function.

#ifdef SPEEDTREE_USE_FOG
float FogValue(float fPoint)
{
    float fFogEnd = g_vFogParams.y;
    float fFogDist = g_vFogParams.z;
    
    return saturate((fFogEnd - fPoint) / fFogDist);
}
#endif


///////////////////////////////////////////////////////////////////////  
//  GroundVS
//
//  Ground vertex shader

SGroundOutput GroundVS(float4 vPosition  : POSITION,
                       float2 vTexCoords : TEXCOORD0)
{
    // this will be fed to the overlay pixel shader
    SGroundOutput sOutput;
    
    // project to screen
    sOutput.vPosition = mul(g_mModelViewProj, vPosition);
    sOutput.vTexCoords.xy = vTexCoords;
    
#ifdef SPEEDTREE_USE_FOG
    sOutput.vTexCoords.z = FogValue(sOutput.vPosition.z);
#endif

    return sOutput;
}


///////////////////////////////////////////////////////////////////////  
//  GroundPS

float4 GroundPS(SGroundOutput In) : COLOR
{
    // simple texture lookup
    float4 texDiffuse = tex2D(samGroundMap, In.vTexCoords.xy);
    float4 vOutput = texDiffuse * g_vGroundColor;
    
#ifdef SPEEDTREE_USE_FOG
    // if fog is active, interpolate between the unfogged color and the fog color
    // based on vertex shader fog value
    float fFogValue = In.vTexCoords.z;
    vOutput.xyz = lerp(g_vFogColor.xyz, vOutput.xyz, fFogValue);
#endif

    return vOutput;
}


///////////////////////////////////////////////////////////////////////  
//  Ground technique

technique Ground
{
    pass P0
    {          
        VS_COMPILE_COMMAND GroundVS( );
        PS_COMPILE_COMMAND GroundPS( );
    }
}


///////////////////////////////////////////////////////////////////////  
//  WindIcon VS Ouput, PS Input

struct SWindIconOutput
{
    float4 vPosition    : POSITION;
    float2 vTexCoords   : TEXCOORD0;
};


///////////////////////////////////////////////////////////////////////  
//  WindIconVS
//
//  Used to render the circular local wind indicator

SWindIconOutput WindIconVS(float4 vPosition  : POSITION,
                           float2 vTexCoords : TEXCOORD0)
{
    // this will be fed to the overlay pixel shader
    SWindIconOutput sOutput;

    vPosition = mul(g_mWindIconBillboardMatrix, vPosition);
    vPosition.xyz *= g_fWindIconScale;
    vPosition.xyz += g_vWindIconPos;
    
    // project to screen
    sOutput.vPosition = mul(g_mModelViewProj, vPosition);
    sOutput.vTexCoords = vTexCoords;

    return sOutput;
}


///////////////////////////////////////////////////////////////////////  
//  WindIconPS

float4 WindIconPS(SWindIconOutput In) : COLOR
{
    float4 texDiffuse = tex2D(samLocalWindEffect, In.vTexCoords);

    return texDiffuse * g_vWindIconColor;
}


///////////////////////////////////////////////////////////////////////  
//  WindIcon technique

technique WindIcon
{
    pass P0
    {          
        VS_COMPILE_COMMAND WindIconVS( );
        PS_COMPILE_COMMAND WindIconPS( );
    }
}





