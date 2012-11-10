#include "MWPSManager.h"

using namespace Myway;

IMPLEMENT_SINGLETON(PS_Manager)

PS_Manager::PS_Manager()
{
    INITIALIZE_SINGLETON();
}

PS_Manager::~PS_Manager()
{
    DestroyAllParticleSystem();

    RenderFactoryMap::Iterator whr0 = _myRenderFactorys.Begin();
    RenderFactoryMap::Iterator end0 = _myRenderFactorys.End();
    EmitterFactoryMap::Iterator whr1 = _myEmitterFactorys.Begin();
    EmitterFactoryMap::Iterator end1 = _myEmitterFactorys.End();
    AffectorFactoryMap::Iterator whr2 = _myAffectorFactorys.Begin();
    AffectorFactoryMap::Iterator end2 = _myAffectorFactorys.End();

    while (whr0 != end0)
    {
        delete whr0->second;
        ++whr0;
    }

    while (whr1 != end1)
    {
        delete whr1->second;
        ++whr1;
    }

    while (whr2 != end2)
    {
        delete whr2->second;
        ++whr2;
    }
}

void PS_Manager::AddFactory(PS_RenderFactory * factory)
{
    assert (_myRenderFactorys.Find(factory->GetType()) == _myRenderFactorys.End());
    _myRenderFactorys.Insert(factory->GetType(), factory);
}

void PS_Manager::AddFactory(PS_EmitterFactory * factory)
{
    assert (_myEmitterFactorys.Find(factory->GetType()) == _myEmitterFactorys.End());
    _myEmitterFactorys.Insert(factory->GetType(), factory);
}

void PS_Manager::AddFactory(PS_AffectorFactory * factory)
{
    assert (_myAffectorFactorys.Find(factory->GetType()) == _myAffectorFactorys.End());
    _myAffectorFactorys.Insert(factory->GetType(), factory);
}

PS_Render * PS_Manager::CreateRender(const String & name, const String & type)
{
    RenderFactoryMap::Iterator whr = _myRenderFactorys.Find(type);
    RenderFactoryMap::Iterator end = _myRenderFactorys.End();

    if (whr != end)
    {
        return whr->second->CreateInstance(name);
    }

    return NULL;
}

PS_Emitter * PS_Manager::CreateEmitter(const String & name, const String & type)
{
    EmitterFactoryMap::Iterator whr = _myEmitterFactorys.Find(type);
    EmitterFactoryMap::Iterator end = _myEmitterFactorys.End();

    if (whr != end)
    {
        return whr->second->CreateInstance(name);
    }

    return NULL;
}

PS_Affector * PS_Manager::CreateAffector(const String & name, const String & type)
{
    AffectorFactoryMap::Iterator whr = _myAffectorFactorys.Find(type);
    AffectorFactoryMap::Iterator end = _myAffectorFactorys.End();

    if (whr != end)
    {
        return whr->second->CreateInstance(name);
    }

    return NULL;
}

ParticleSystem * PS_Manager::CreateParticleSystem(const String & name)
{
    assert (GetParticleSystem(name) == NULL);

    ParticleSystem * ps = new ParticleSystem(name);

    _myParticleSystems.Insert(name, ps);

    return ps;
}

ParticleSystem * PS_Manager::CreateParticleSystem(const String & name, const String source)
{
    assert (GetParticleSystem(name) == NULL);

    return NULL;
}

ParticleSystem * PS_Manager::GetParticleSystem(const String & name)
{
    ParticleSystemMap::Iterator whr = _myParticleSystems.Find(name);
    ParticleSystemMap::Iterator end = _myParticleSystems.End();

    return whr != end ? whr->second : NULL;
}

void PS_Manager::DestroyParticleSystem(ParticleSystem * ps)
{
    DestroyParticleSystem(ps->GetName());
}

void PS_Manager::DestroyParticleSystem(const String & name)
{
    ParticleSystemMap::Iterator whr = _myParticleSystems.Find(name);
    ParticleSystemMap::Iterator end = _myParticleSystems.End();

    assert (whr != end);

    delete whr->second;
    _myParticleSystems.Erase(whr);
}

void PS_Manager::DestroyAllParticleSystem()
{
    ParticleSystemMap::Iterator whr = _myParticleSystems.Begin();
    ParticleSystemMap::Iterator end = _myParticleSystems.End();

    while (whr != end)
    {
        delete whr->second;
        ++whr;
    }

    _myParticleSystems.Clear();
}