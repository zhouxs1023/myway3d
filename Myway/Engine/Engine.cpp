#include "Engine.h"

using namespace Myway;

IMP_SLN(Engine);

Engine::Engine()
: mRenderSystem(NULL)
, mInputSystem(NULL)
, mAudioSystem(NULL)
, mRenderScheme(NULL)
, mSkeletonManager(NULL)
, mMeshManager(NULL)
, mDllManager(NULL)
, mResourceManager(NULL)
, mLogSystem(NULL)
, mLastFrameTime(0)
, mFrameTime(0)
, mTime2PI(0)
, mLastFPSCalcTime(0)
, mFPS(0)
{
    INIT_SLN;
}

Engine::~Engine()
{
    SHUT_SLN;
}

void Engine::Init(const DeviceProperty * pDeviceProperty,
                        const char * sConfigs,
                        const char * sPlugins)
{
    Memory::Init();
    System::Init();
    Math::Init();

    mLogSystem = new LogSystem();
    mDllManager = new DllManager();
    mResourceManager = new ResourceManager();

    ResourceManager::Instance()->AddArchiveFactory(new FileSystemFactory());
    ResourceManager::Instance()->AddArchiveFactory(new ArchiveZipFactory());

    mDeviceProperty = *pDeviceProperty;

    ParseConfig(sConfigs);
    ParsePlugin(sPlugins);

    mSkeletonManager = new SkeletonManager();
    mMeshManager = new MeshManager();

    PluginManager::Instance()->InstallAll();

    mShaderLibManager = new ShaderLibManager();

    mWorld = new World();

    mRenderHelper = new RenderHelper();
    mRenderScheme = new RS_Scheme();
}

void Engine::Shutdown()
{
    safe_delete(mRenderScheme);
    safe_delete(mRenderHelper);

    safe_delete(mWorld);
    safe_delete(mMeshManager);
    safe_delete(mSkeletonManager);

    safe_delete(mShaderLibManager);

	safe_delete(mInputSystem);
    safe_delete(mAudioSystem);
    safe_delete(mRenderSystem);

    PluginManager::Instance()->UninstallAll();

    safe_delete(mResourceManager);
    safe_delete(mDllManager);
    safe_delete(mLogSystem);

    Math::DeInit();
    System::DeInit();
    Memory::DeInit();
}

void Engine::Resize(int w, int h)
{
    if (mDeviceProperty.Width == w &&
        mDeviceProperty.Height == h)
        return ;

    mDeviceProperty.Width = w;
    mDeviceProperty.Height = h;

    mRenderSystem->OnResize();

    World::Instance()->MainCamera()->SetAspect((float)w / h);

    RenderEvent::OnResize.Call();
}

void Engine::SetRenderSystem(RenderSystem * pRenderSystem)
{
    d_assert (!mRenderSystem);

    mRenderSystem = pRenderSystem;
}

void Engine::SetInputSystem(InputSystem * pInputSystem)
{
    d_assert (!mInputSystem);

    mInputSystem = pInputSystem;
}

void Engine::SetAudioSystem(AudioSystem * pAudioSystem)
{
    d_assert (!mAudioSystem);

    mAudioSystem = pAudioSystem;
}

void Engine::ParseConfig(const char * sFileName)
{
    xml_doc doc;

    char * data = XmlHelper::LoadXmlFromFile(doc, sFileName);

    xml_node * node = doc.first_node()->first_node("resources");

    while (node)
    {
        const char * type = node->first_attribute("type")->value();
        const char * path = node->first_attribute("path")->value();
        ResourceManager::Instance()->AddArchive(path, type);

        node = node->next_sibling("resources");
    }

    ResourceManager::Instance()->Init();
    LogSystem::Instance()->Init("Log.txt");

    safe_free(data);
}

void Engine::ParsePlugin(const char * sFileName)
{
    xml_doc doc;

    char * data = XmlHelper::LoadXmlFromFile(doc, sFileName);

    xml_node * node = doc.first_node()->first_node("plugin");

    while (node)
    {
        const char * pluginName = node->first_attribute("name")->value();
        DllManager::Instance()->Load(pluginName);

        node = node->next_sibling("plugin");
    }

    safe_free(data);
}

void Engine::Run()
{
    profile_code();

    float time = GetTime();

    mFrameTime = time - mLastFrameTime;
    mLastFrameTime = time;

    mTime2PI += mFrameTime;
    while (mTime2PI > Math::PI_2)
        mTime2PI -= Math::PI_2;

    if (time - mLastFPSCalcTime > 1.0f)
    {
        mFPS = mFrameTime ? 1.0f / mFrameTime : 0;
        mLastFPSCalcTime = time;
    }

    mWorld->UpdateFrame();
}

float Engine::GetFrameTime()
{
    return mFrameTime;
}

float Engine::GetFPS()
{
    return mFPS;
}

int Engine::GetPrimitiveCount()
{
    return mRenderSystem->GetFramePrimitiveCount();
}

int Engine::GetBatchCount()
{
    return mRenderSystem->GetFrameBatchCount();
}

float Engine::GetTime()
{
    return System::InternalTime.ElapsedTime();
}

float Engine::GetTime2PI()
{
    return mTime2PI;
}

const DeviceProperty * Engine::GetDeviceProperty() const
{
    return &mDeviceProperty;
}
