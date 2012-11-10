#pragma once

#include "MWParticle.h"

namespace Myway
{

struct Particle;
class ParticleSystem;
class PS_Render;
class PS_Emitter;
class PS_Affector;

class RenderQueue;

class MW_ENTRY PS_System : public AllocObj
{
    typedef Vector<Particle>    ParticlePool;
    typedef Stack<Particle*>    ParticleStack; 

public:
    typedef List<PS_Emitter*>                   EmitterList;
    typedef List<PS_Affector*>                  AffectorList;
    typedef List<Particle*>                     ParticleList;
    typedef Visitor<EmitterList::Iterator>      EmitterVisitor;
    typedef Visitor<AffectorList::Iterator>     AffectorVisitor;
    typedef Visitor<ParticleList::Iterator>     ParticleVisitor;

public:
    PS_System(ParticleSystem * parent, const String & name);
    ~PS_System();

    const String & GetName() const;
    ParticleSystem * GetParent() const;

    void Update(float fElapsedTime);

    PS_Render * CreateRender(const String & name, const String & type);
    PS_Emitter * CreateEmitter(const String & name, const String & type);
    PS_Affector * CreateAffector(const String & name, const String & type);

    PS_Emitter * GetEmitter(const String & name);
    PS_Affector * GetAffector(const String & name);

    void DestroyEmitter(const String & name);
    void DestroyAffector(const String & name);

    PS_Render * GetRender();
    AffectorVisitor GetAffectors();
    EmitterVisitor GetEmitters();
    ParticleVisitor GetParticles();
    int GetParticleSize() const;

    void AddRenderQueue(RenderQueue * rq);

    void SetQuota(int quota);
    int GetQuota() const;

    void SetEnable(bool enable);
    bool GetEnable() const;

protected:
    void _Death(float fElapsedTime);
    void _Affect(float fElapsedTime);
    void _Emit(float fElapsedTime);

protected:
    Particle * _Quest();
    void _Free(Particle * p);

protected:
    ParticleSystem *        _myParent;
    String                  _myname;
    PS_Render *             _myRender;
    EmitterList             _myEmitters;
    AffectorList            _myAffectors;
    ParticleList            _myParticles;

    ParticlePool            _myPool;
    ParticleStack           _myFree;

    int                  _myQuota;
    bool                    _myEnable;
};

}