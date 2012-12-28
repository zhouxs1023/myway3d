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

        static Event OnLostDevice;
        static Event OnResetDevice;

        static Event OnResize;

		static Event OnBeginRender;

		static Event OnAfterRenderSolid;
		static Event OnAfterDeffererShading;
		static Event OnAfterTerrainRender;

		static Event OnAfterRender;
		static Event OnDebugRender;
    };
}
