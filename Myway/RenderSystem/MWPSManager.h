#pragma once

#include "MWPSRender.h"
#include "MWPSEmitter.h"
#include "MWPSAffector.h"
#include "MWParticleSystem.h"

namespace Myway
{

class MW_ENTRY PS_Manager : public AllocObj
{
    DECLARE_SINGLETON(PS_Manager)

protected:
    typedef Map<String, PS_RenderFactory *>     RenderFactoryMap;
    typedef Map<String, PS_AffectorFactory *>   AffectorFactoryMap;
    typedef Map<String, PS_EmitterFactory *>    EmitterFactoryMap;

public:
    typedef HashMap<String, ParticleSystem *, String::hash_t>   ParticleSystemMap;

public:
    PS_Manager();
    ~PS_Manager();

    void AddFactory(PS_RenderFactory * factory);
    void AddFactory(PS_EmitterFactory * factory);
    void AddFactory(PS_AffectorFactory * factory);

    PS_Render * CreateRender(const String & name, const String & type);
    PS_Emitter * CreateEmitter(const String & name, const String & type);
    PS_Affector * CreateAffector(const String & name, const String & type);

    ParticleSystem * CreateParticleSystem(const String & name);
    ParticleSystem * CreateParticleSystem(const String & name, const String source);
    ParticleSystem * GetParticleSystem(const String & name);
    void DestroyParticleSystem(ParticleSystem * ps);
    void DestroyParticleSystem(const String & name);
    void DestroyAllParticleSystem();

protected:
    RenderFactoryMap    _myRenderFactorys;
    AffectorFactoryMap  _myAffectorFactorys;
    EmitterFactoryMap   _myEmitterFactorys;

    ParticleSystemMap   _myParticleSystems;
};

}