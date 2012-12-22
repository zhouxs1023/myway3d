///////////////////////////////////////////////////////////////////////  
//  SpeedTree Shaders File
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
//
//  These shaders were written to be used as HLSL FX or CgFX files for the
//  following platforms/APIs:
//      - Windows DirectX 9.0c
//      - Windows OpenGL 1.4+
//      - Xbox(R) 360
//      - PLAYSTATION(R)3
//
//  SpeedTree.fx is the main fx file that's compiled by the application.  It
//  includes several other fx files that define shaders for each of the geometry
//  types composing a SpeedTree model.
//
//  There are several macros used throughout these fx files that are defined
//  at runtime by the application (they're passed in with the compile command).
//  They are listed alphabetically below:
//
//  SPEEDTREE_OPENGL
//  Defined for Windows PC and PLAYSTATION3 builds.  When not defined, Windows
//  DirectX 9.0c and Xbox 360 builds are assumed.  It is used in most of the
//  fx files.
//
//  SPEEDTREE_DIRECTX9
//  Defined for Windows PC DirectX 9.0c builds only - not defined for Xbox 360 builds.
//
//  PS3
//  Used to specify the unique vertex and pixel shader build types
//  (sce_vp_rsx and sce_fp_rsx).
//
//  VS_COMPILE_COMMAND and PS_COMPILE_COMMAND (defined in CompileCommands.fx)
//  Three of the four platforms have unique shader compile targets and 
//  commands. The complete compile commands, including the targets, are stored 
//  in these macros.  They're used in most of the fx files.
//
//  SPEEDTREE_LOW_COMPILE_TARGET
//  When compiling to lower-end compile targets like vs_1_1 and ps_1_1, this should
//  be #defined.  It turns off some of the features that are not possible in these
//  more restrictive profiles (ps_1_1 being the most restrictive).  ** Please note
//  that as of the Dec 2006 DirectX 9.0 SDK release (and the SpeedTreeRT 4.1 release 
//  that followed it), fxc will not compile some of the SpeedTree shaders to this 
//  target (specifically anything under ps_2_0), even with the /LD the fxc
//  documentation recommends using for lower targets. Further, when the /LD flag is 
//  given, the compiler fails on half of the pixel shaders. It reports "compilation 
//  failed; no code generated."
//
//  SPEEDTREE_BRANCH_DETAIL_LAYER
//  The sample branch shaders include support for three texture layers:  diffuse
//  map, normal map, and detail map.  Support of the detail map layer is optional 
//  and is enabled by defining this macro.  It is used only in Branch.fx.
//
//  SPEEDTREE_BRANCH_NORMAL_MAPPING
//  Used in the branch shaders to support standard normal mapping on the bark
//  textures.
//
//  SPEEDTREE_FROND_NORMAL_MAPPING
//  Used in the frond shaders to support single directional-light normal mapping 
//  on the composite leaf/frond texture.
//
//  SPEEDTREE_LEAF_NORMAL_MAPPING
//  Used in the leaf mesh and leaf card shaders to support single directional-light 
//  normal mapping on the composite leaf/frond texture.
//
//  SPEEDTREE_BRANCH_FADING
//  Used to activate the intermediate fizzling code in the branch and frond shaders.  
//  It will employ a float hint per vertex to adjust the alpha fizzling effect in 
//  order to reduce popping between discrete branch and frond LOD meshes.
//
//  SPEEDTREE_SELF_SHADOW_LAYER
//  Enables self-shadow texture layer support in the branch and frond shaders.
//
//  SPEEDTREE_USE_FOG
//  The use of fog in a pixel shader is a non-trivial expense.  It can be toggled
//  using this macro.  It is used in most of the fx files.
//
//  SPEEDTREE_BILLBOARD_NORMAL_MAPPING
//  Used in the billboard shaders to support a special type of normal map that
//  allows very smooth transitions from 3D trees into dynamically lit billboard
//  images.
//
//  SPEEDTREE_NUM_360_IMAGES
//  This macro defines maximum number of individual 360-degree images that can
//  uploaded per tree.  It is used only in Billboard.fx
//
//  SPEEDTREE_MAX_NUM_LEAF_ANGLES
//  Each of the leaves in a particular tree model point to one of several
//  different rock and rustle angles.  This allows independent wind behavior for
//  the leaves, leading to a more realistic wind effect.  This macro is set to 
//  define the number of leaf rock/rustle angles that can be uploaded.  It is
//  used only in Leaf.fx.
//
//  SPEEDTREE_NUM_WIND_MATRICES
//  Each main branch, and its corresponding children (smaller branches, leaves) 
//  refer to a single wind matrix to define its sway behavor.  This macro defines
//  the total number of wind matrices uploaded per frame. It is used in Utility.fx
//  where the wind function is housed, which is called by Branch.fx, Frond.fx, and
//  Leaf.fx.
//
//  SPEEDTREE_360_BILLBOARDS
//  Most applications should use this macro, which enables the billboard vertex
//  shaders BillboardVS1 and BillboardVS2 to compute the fade alpha values during
//  360 image transitions.  Disabling the macro will fade the shaders only compute
//  alpha values for the fade between 3D and billboard.
//
//  SPEEDTREE_HORZ_BILLBOARDS
//  Enables horizontal billboard rendering.  SPT files must be exported from 
//  SpeedTreeCAD with horizontal billboard support enabled.
//
//  SPEEDTREE_TWO_WEIGHT_WIND
//  In versions prior to 4.0, SpeedTree used one wind weight per vertex.  To enable
//  the newer 4.0-style two-weight wind, this must be #defined.
//
//  SPEEDTREE_UPVECTOR_Y
//  Should be enabled for applications that use +Y as the up axis (as opposed to +Z)
//
//  SPEEDTREE_ACCURATE_WIND_LIGHTING
//  This macro is used to control whether the tree's normals, binormals, and tangents
//  are transformed with the wind in addition to the vertex's coordinates.  Disabling
//  it will yield a modest performance increase in exchange for a subtle change in the
//  lighting behavior under windy conditions.
//
//  SPEEDTREE_WORLD_TRANSLATE
//  When defined, the forest geometry is translated to the camera instead of the camera 
//  being translated through the forest.  This helps to reduce a lot of the depth buffer 
//  noise common with large world coordinates.
//
//  SPEEDTREE_FADE_VERT_WITH_HORZ_BILLBOARDS
//  The system will crossfade the vertical and horizontal billboards based on the
//  camera pitch.


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
//  Common texture filter 

#define SPEEDTREE_FILTER Linear


///////////////////////////////////////////////////////////////////////  
//  Common Global Variables
//
//  These are all global variables that are shared by more than one of
//  the main shader types (branch, frond, leaf, and billboard).  The constant
//  registers are specified after many of the global parameters in order
//  to ensure that the variables will be successfully shared even when the
//  shaders are compiled off-line separately.
//
//  Note: What we call "composite maps" or "composite textures" are also known
//        as "texture atlases"

// textures
texture     g_tCompositeDiffuseLeafMap;                 // this composite texture contains all of the leaf and frond images
texture     g_tCompositeNormalLeafMap;                  // this composite texture contains all of the leaf and frond images
texture     g_tSelfShadowMap;                           // this composite texture contains all of the self-shadow maps (greyscale)

// wind
float       g_fWindMatrixOffset REG(6);                 // keeps two instances of the same tree model from using the same wind matrix (range: [0,SPEEDTREE_NUM_WIND_MATRICES])
#ifdef SPEEDTREE_WORLD_TRANSLATE
float3      g_vCameraPos REG(7);
#endif
float4x4    g_amWindMatrices[SPEEDTREE_NUM_WIND_MATRICES] REG(22);// houses all of the wind matrices shared by all geometry types

// lighting & materials
float3      g_vLightDir REG(9);                         // normalized light direction (shaders assume a single light source)
float4      g_vLightDiffuse REG(10);                    // light's diffuse color
float4      g_vMaterialDiffuse REG(11);                 // active material's diffuse color (set differently for branches, fronds, leaves, and billboards)
float       g_fDiffuseScale REG(12);                    // mirrors the diffuse scalar value in CAD, used to help combat the darkening effect of multiple texture layers
float4      g_vMaterialAmbient REG(13);                 // active material's ambient color (set differently for branches, fronds, leaves, and billboards)
float       g_fAmbientScale REG(14);                    // mirrors the ambient scalar value in CAD, used to help combat the darkening effect of multiple texture layers
float       g_fLeafLightingAdjust REG(15);              // used to clamp the dot product operation during the leaf vertex lighting operations

// other
float4x4    g_mModelViewProj REG(0);                    // composite modelview/projection matrix
float3      g_vFogColor REG(16);                        // color of fog (and likely the background)
float3      g_vFogParams REG(17);                       // used for fog distance calcs: .x = fog start, .y = fog end, .z = end - start
float4      g_vTreePos REG(4);                          // each tree is in a unique position and rotation: .xyz = pos, .w = rotation
float4      g_vTreeRotationTrig REG(19);                // stores (sin, cos, -sin, 0.0) for an instance's rotation angle (optimizes rotation code)
float2      g_vCameraAngles REG(8);                     // shared by Billboard.fx and Leaf.fx - stores camera azimuth and pitch for billboarding computations
float4      g_vCameraAzimuthTrig REG(20);               // stores (sin, cos, -sin, 0.0) for the camera azimuth angle for optimized rotation in billboard vs
#ifdef SPEEDTREE_BRANCH_FADING
float2      g_vBranchLodValues REG(21);                 // .x = current LOD, .y = transition radius
#endif

// constants
const float c_fClearAlpha = 255.0f;                     // alpha testing, 255 means not visible
const float c_fOpaqueAlpha = 84.0f;                     // alpha testing, 84 means fully visible
const float c_fAlphaSpread = 171.0f;                    // 171 = 255 - 84
const float c_fTwoPi = 6.28318530f;                     // 2 * pi


///////////////////////////////////////////////////////////////////////  
//  Common Texture Samplers
//
//  These are samplers shared by more than one pixel shader

sampler2D samCompositeDiffuseLeafMap : register(s0) = sampler_state
{
    Texture = <g_tCompositeDiffuseLeafMap>;
#ifdef SPEEDTREE_DIRECTX9
    MagFilter = SPEEDTREE_FILTER;
    MipFilter = SPEEDTREE_FILTER;
    MinFilter = SPEEDTREE_FILTER;
#endif
};

sampler2D samCompositeNormalLeafMap : register(s1) = sampler_state
{
    Texture = <g_tCompositeNormalLeafMap>;
#ifdef SPEEDTREE_DIRECTX9
    MagFilter = SPEEDTREE_FILTER;
    MipFilter = SPEEDTREE_FILTER;
    MinFilter = SPEEDTREE_FILTER;
#endif
};

#ifdef SPEEDTREE_SELF_SHADOW_LAYER
sampler2D samSelfShadowMap : register(s2) = sampler_state
{
    Texture = <g_tSelfShadowMap>;
#ifdef SPEEDTREE_DIRECTX9
    MagFilter = SPEEDTREE_FILTER;
    MipFilter = SPEEDTREE_FILTER;
    MinFilter = SPEEDTREE_FILTER;
#endif
};
#endif

#include "Utility.fx"
#include "Branch.fx"
#include "Leaf.fx"
#include "Frond.fx"
#include "Billboard.fx"


