#pragma once

#include "MWCore.h"


namespace Myway {

    class MW_ENTRY RenderEvent
    {
    public:
		static Event OnEngineInit;
		static Event OnEngineShutdown;

        static Event OnPreUpdateScene;
        static Event OnPostUpdateScene;

        static Event OnPreRenderEvent;
        static Event OnPostRenderEvent;

        static Event OnPreVisibleCull;
        static Event OnPostVisibleCull;
		static Event OnFilterCullResult;
		static Event OnOptimizeMirrorCullResult;

        static Event OnLostDevice;
        static Event OnResetDevice;

        static Event OnResize;

		static Event OnBeginRender;

		static Event OnRenderSolid1;
		static Event OnRenderSolid2;
		static Event OnRenderSolid3;

		static Event OnAfterRenderSolid;
		static Event OnAfterRenderTrans;
		static Event OnAfterDefferedShading;
		static Event OnAfterTerrainRender;

		static Event OnMirrorRenderSolid1;
		static Event OnMirrorRenderSolid2;
		static Event OnMirrorRenderSolid3;

		static Event OnRenderGUI1;
		static Event OnRenderGUI2;
		static Event OnRenderGUI3;

		static Event OnAfterRender;
		static Event OnDebugRender;
    };
}
