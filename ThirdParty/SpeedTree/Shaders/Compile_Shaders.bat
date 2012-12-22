@echo off
::
::  SpeedTree Shader Compilation Utility
::
::  *** INTERACTIVE DATA VISUALIZATION (IDV) CONFIDENTIAL AND PROPRIETARY INFORMATION ***
::
::  This software is supplied under the terms of a license agreement or
::  nondisclosure agreement with Interactive Data Visualization, Inc. and
::  may not be copied, disclosed, or exploited except in accordance with 
::  the terms of that agreement.
::
::      Copyright (c) 2003-2008 IDV, Inc.
::      All rights reserved in all media.
::
::      IDV, Inc.
::      Web: http://www.idvinc.com
::
::  *** Release Version 4.2 ***

setlocal


if "%1" == "" goto usage

:: Set up #defines to configure shaders
set DEFINES=-DSPEEDTREE_NUM_WIND_MATRICES=6
set DEFINES=%DEFINES% -DSPEEDTREE_MAX_NUM_LEAF_ANGLES=8
set DEFINES=%DEFINES% -DSPEEDTREE_NUM_360_IMAGES=64
set DEFINES=%DEFINES% -DSPEEDTREE_BRANCH_DETAIL_LAYER
set DEFINES=%DEFINES% -DSPEEDTREE_BRANCH_NORMAL_MAPPING
set DEFINES=%DEFINES% -DSPEEDTREE_FROND_NORMAL_MAPPING
set DEFINES=%DEFINES% -DSPEEDTREE_LEAF_NORMAL_MAPPING
set DEFINES=%DEFINES% -DSPEEDTREE_USE_FOG
set DEFINES=%DEFINES% -DSPEEDTREE_SELF_SHADOW_LAYER
set DEFINES=%DEFINES% -DSPEEDTREE_360_BILLBOARDS
set DEFINES=%DEFINES% -DSPEEDTREE_BILLBOARD_NORMAL_MAPPING
set DEFINES=%DEFINES% -DSPEEDTREE_HORZ_BILLBOARDS
set DEFINES=%DEFINES% -DSPEEDTREE_BRANCH_FADING
::set DEFINES=%DEFINES% -DSPEEDTREE_ACCURATE_WIND_LIGHTING
set DEFINES=%DEFINES% -DSPEEDTREE_FADE_VERT_WITH_HORZ_BILLBOARDS
set DEFINES=%DEFINES% -DSPEEDTREE_WORLD_TRANSLATE
set DEFINES=%DEFINES% -DSPEEDTREE_EXTRAS
set DEFINES=%DEFINES% -DSPEEDTREE_UPVECTOR_Z
::set DEFINES=%DEFINES% -DSPEEDTREE_UPVECTOR_Y


if "%1" == "PS3" goto Ps3Cg
if "%1" == "FX" goto FX
if "%1" == "Cg" goto WinCg
goto usage

::
:: ----------- PS3 Cg Compiler -----------------------------------------------------------------------------------------------
::

:Ps3Cg
set VP_PROFILE=sce_vp_rsx
set FP_PROFILE=sce_fp_rsx
set COMPILER=sce-cgc.exe
set DEFINES=%DEFINES% -DSPEEDTREE_OPENGL
set DEFINES=%DEFINES% -DPS3

::Vertex Programs
echo. 
echo *** Compiling BranchVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry BranchVS -o PS3/BranchVS.vpo -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling FrondVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry FrondVS -o PS3/FrondVS.vpo -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling LeafCardVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry LeafCardVS -o PS3/LeafCardVS.vpo -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling LeafMeshVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry LeafMeshVS -o PS3/LeafMeshVS.vpo -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling Billboard1VS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry Billboard1VS -o PS3/Billboard1VS.vpo -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling Billboard2VS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry Billboard2VS -o PS3/Billboard2VS.vpo -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling HorzBillboardVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry HorzBillboardVS -o PS3/HorzBillboardVS.vpo -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling OverlayVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry OverlayVS -o PS3/OverlayVS.vpo -v -profile %VP_PROFILE% Overlay.fx
echo. 
echo *** Compiling GroundVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry GroundVS -o PS3/GroundVS.vpo -v -profile %VP_PROFILE% Environment.fx
echo.
echo *** Compiling WindIconVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry WindIconVS -o PS3/WindIconVS.vpo -v -profile %VP_PROFILE% Environment.fx

::Fragment Programs
echo. 
echo *** Compiling BranchPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry BranchPS -o PS3/BranchPS.fpo -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling FrondPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry FrondPS -o PS3/FrondPS.fpo -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling LeafCardPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry LeafPS -o PS3/LeafCardPS.fpo -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling LeafMeshPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry LeafPS -o PS3/LeafMeshPS.fpo -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling BillboardPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry BillboardPS -o PS3/BillboardPS.fpo -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling OverlayPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry OverlayPS -o PS3/OverlayPS.fpo -v -profile %FP_PROFILE% Overlay.fx
echo. 
echo *** Compiling GroundPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry GroundPS -o PS3/GroundPS.fpo -v -profile %FP_PROFILE% Environment.fx
echo. 
echo *** Compiling WindIconPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry WindIconPS -o PS3/WindIconPS.fpo -v -profile %FP_PROFILE% Environment.fx

::Convert all files to single ELF file
cgnv2elf PS3 SpeedTreeShaders.cgelf

goto complete


::
:: ----------- Windows Cg Compiler --------------------------------------------------------------------------------------------
::

:WinCg
set VP_PROFILE=arbvp1
set FP_PROFILE=arbfp1
set COMPILER=cgc
set DEFINES=%DEFINES% -DSPEEDTREE_OPENGL


::Vertex Programs
echo. 
echo *** Compiling BranchVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry BranchVS -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling FrondVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry FrondVS -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling LeafCardVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry LeafCardVS -v -profile %VP_PROFILE% SpeedTree.fx
echo.
echo *** Compiling LeafMeshVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry LeafMeshVS -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling Billboard1VS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry Billboard1VS -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling Billboard2VS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry Billboard2VS -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling HorzBillboardVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry HorzBillboardVS -v -profile %VP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling OverlayVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry OverlayVS -v -profile %VP_PROFILE% Overlay.fx
echo. 
echo *** Compiling GroundVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry GroundVS -v -profile %VP_PROFILE% Environment.fx
echo. 
echo *** Compiling WindIconVS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry WindIconVS -v -profile %VP_PROFILE% Environment.fx

::Fragment Programs
echo. 
echo *** Compiling BranchPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry BranchPS -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling FrondPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry FrondPS -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling LeafPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry LeafPS -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling BillboardPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry BillboardPS -v -profile %FP_PROFILE% SpeedTree.fx
echo. 
echo *** Compiling OverlayPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry OverlayPS -v -profile %FP_PROFILE% Overlay.fx
echo. 
echo *** Compiling GroundPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry GroundPS -v -profile %FP_PROFILE% Environment.fx
echo. 
echo *** Compiling WindIconPS ***
%COMPILER% %DEFINES% -fastmath -fastprecision -entry WindIconPS -v -profile %FP_PROFILE% Environment.fx


goto complete


::
:: ----------- DirectX FX Compiler (Windows/Xenon version) --------------------------------------------------------------
::

:FX
set VP_PROFILE=vs_2_0
:: there are a lot of problems with pixel targets below 2.0 in the Dec 2006 DK SDK
set FP_PROFILE=ps_2_0
set COMPILER=fxc
set DEFINES=%DEFINES% -DSPEEDTREE_DIRECTX9
:: if vs_1_1/ps_1_1, make sure SPEEDTREE_LOW_COMPILE_TARGET is #defined
:: set DEFINES=%DEFINES% -DSPEEDTREE_LOW_COMPILE_TARGET

::Vertex Programs
echo. 
echo *** Compiling BranchVS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /EBranchVS SpeedTree.fx
echo. 
echo *** Compiling FrondVS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /EFrondVS SpeedTree.fx
echo. 
echo *** Compiling LeafCardVS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /ELeafCardVS SpeedTree.fx
echo. 
echo *** Compiling LeafMeshVS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /ELeafMeshVS SpeedTree.fx
echo. 
echo *** Compiling Billboard1VS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /EBillboard1VS SpeedTree.fx
echo. 
echo *** Compiling Billboard2VS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /EBillboard2VS SpeedTree.fx
echo. 
echo *** Compiling HorzBillboardVS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /EHorzBillboardVS SpeedTree.fx
echo. 
echo *** Compiling OverlayVS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /EOverlayVS Overlay.fx
echo. 
echo *** Compiling WindIconVS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /EWindIconVS Environment.fx
echo. 
echo *** Compiling GroundVS ***
%COMPILER% %DEFINES% /T%VP_PROFILE% /EGroundVS Environment.fx


::Fragment Programs
echo. 
echo *** Compiling BranchPS ***
%COMPILER% %DEFINES% /T%FP_PROFILE% /EBranchPS SpeedTree.fx
echo. 
echo *** Compiling FrondPS ***
%COMPILER% %DEFINES% /T%FP_PROFILE% /EFrondPS SpeedTree.fx
echo. 
echo *** Compiling LeafPS ***
%COMPILER% %DEFINES% /T%FP_PROFILE% /ELeafPS SpeedTree.fx
echo. 
echo *** Compiling BillboardPS ***
%COMPILER% %DEFINES% /T%FP_PROFILE% /EBillboardPS SpeedTree.fx
echo. 
echo *** Compiling OverlayPS ***
%COMPILER% %DEFINES% /T%FP_PROFILE% /EOverlayPS Overlay.fx
echo. 
echo *** Compiling WindIconPS ***
%COMPILER% %DEFINES% /T%FP_PROFILE% /EWindIconPS Environment.fx
echo. 
echo *** Compiling GroundPS ***
%COMPILER% %DEFINES% /T%FP_PROFILE% /EGroundPS Environment.fx


goto Complete


:usage

echo SpeedTree 4.2 Shader Compilation Utility (c) 2006-2008
echo Usage:
echo     Compile_Shaders.bat [PS3 or FX or Cg]
echo        PS3 = sce-cgc.exe with targets sce_vp_rsx and sce_fp_rsx
echo        FX = fxc.exe with targets vs_3_0 and ps_3_0
echo        Cg = cgc.exe with targets arbvp1 and arbfp1


:Complete
pause
