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

#ifdef SPEEDTREE_OPENGL
    // arbvp1/arbfp1 represent the lowest (and broadest) profile that the SpeedTree reference shaders 
    // will work under; note that the number of 360-degree billboard images will be restricted under 
    // this particular compile target to about 60; increase to high target like vp30/fp30 for more
    #define VS_COMPILE_COMMAND VertexProgram = compile arbvp1
    #define PS_COMPILE_COMMAND FragmentProgram = compile arbfp1
#else
    // if using vs_1_1/ps_1_1, see next comment section below
    #define VS_COMPILE_COMMAND VertexShader = compile vs_2_0
    #define PS_COMPILE_COMMAND PixelShader = compile ps_2_0
#endif


//  Comment in SPEEDTREE_LOW_COMPILE_TARGET when compiling to lower targets like vs_1_1/ps_1_1 and
//  vp20/fp20.  It will make sure some of the more advanced shader effects are disabled.
//  Also #undef SPEEDTREE_BRANCH_NORMAL_MAPPING in the C++ project.

//#define SPEEDTREE_LOW_COMPILE_TARGET
#ifdef SPEEDTREE_LOW_COMPILE_TARGET
#undef SPEEDTREE_BRANCH_NORMAL_MAPPING
#undef SPEEDTREE_FROND_NORMAL_MAPPING
#undef SPEEDTREE_LEAF_NORMAL_MAPPING
#undef SPEEDTREE_BRANCH_FADING
#undef SPEEDTREE_BILLBOARD_NORMAL_MAPPING
#endif
