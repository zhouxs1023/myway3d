#pragma once

#include "MWCore.h"
#include "MWMath.h"
#include "MWWorld.h"
#include "MWEntity.h"
#include "MWMaterial.h"
#include "MWMesh.h"
#include "MWMeshManager.h"
#include "MWSkeletonManager.h"
#include "MWResourceManager.h"

#include "MWRenderScheme.h"
#include "MWRenderScheme_Prz.h"
#include "MWRenderHelper.h"

#include "MWInputSystem.h"
#include "MWAudioSystem.h"
#include "MWRenderSystem.h"
#include "MWRenderEvent.h"
#include "MWPhysics.h"

#include "MWEnvironment.h"


namespace Myway
{

enum RENDER_SYSTEM_TYPE
{
    RENDER_DIRECT3D9,
    RENDER_OPENGL,

    MW_ALIGN_ENUM(RENDER_SYSTEM_TYPE)
};

class MW_ENTRY Engine : public AllocObj
{
    DECLARE_SINGLETON(Engine);

    typedef Map<TString128, RenderSystem*> RenderSystemMap;

public:
    Engine();
    ~Engine();

    void Init(const DeviceProperty * pDeviceProperty,
              const char * sConfigs,
              const char * sPlugins);
    void Shutdown();

    void Resize(int w, int h);
   
    void SetRenderSystem(RenderSystem * pRenderSystem);
    void SetInputSystem(InputSystem * pInputSystem);
    void SetAudioSystem(AudioSystem * pAudioSystem);

	void SetPhyWorld(IPhyWorld * phyWorld) { mPhyWorld = phyWorld; }
	IPhyWorld * GetPhyWorld() { return mPhyWorld; }

    void Run();
    float GetFrameTime();
    float GetTime2PI();
    float GetTime();
    float GetFPS();
    int GetPrimitiveCount();
    int GetBatchCount();

    const TString128 & GetAABBMaterial();

    const DeviceProperty * GetDeviceProperty() const;

protected:
    void ParseConfig(const char * sFileName);
    void ParsePlugin(const char * sFileName);

protected:
    DeviceProperty          mDeviceProperty;

    AudioSystem *           mAudioSystem;
	InputSystem *			mInputSystem;
    RenderSystem *          mRenderSystem;
	IPhyWorld *				mPhyWorld;

    World *                 mWorld;
    MeshManager *           mMeshManager;
    DllManager *            mDllManager;
    ResourceManager *       mResourceManager;
    LogSystem *             mLogSystem;

    ShaderLibManager *      mShaderLibManager;

    RenderScheme *          mRenderScheme;
    RenderHelper *          mRenderHelper;
    
    float                   mLastFrameTime;
    float                   mFrameTime;
    float                   mTime2PI;
    float                   mLastFPSCalcTime;
    float                   mFPS;
};

}