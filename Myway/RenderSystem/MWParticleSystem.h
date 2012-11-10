#pragma once

#include "MWRenderer.h"
#include "MWMover.h"
#include "MWParticle.h"

namespace Myway
{

class PS_System;

class MW_ENTRY ParticleSystem : public Mover
{
    typedef List<PS_System*>    PS_SystemList;

public:
    typedef Visitor<PS_SystemList::Iterator> PS_SystemVisitor;

public:
    static const guid128 Guid;

public:
    ParticleSystem(const String & name);
    virtual ~ParticleSystem();

    const String & GetName() const;
    const String & GetSourceName() const;

    virtual void GetWorldPosition(Vec3 * pos);
    virtual void GetWorldTransform(Mat4 * form);
    virtual LightList * GetLightList();

    virtual const guid128 & GetGuid();

    virtual void NotifyCamera(Camera * cam);
    virtual void UpdateMover();
    virtual void AddRenderQueue(RenderQueue * rq);

    Camera * GetCurrentCamera();

    PS_System * CreateSystem(const String & name);
    PS_System * GetSystem(const String & name);
    void RemoveSystem(const String & name);
    void RemoveSystem(PS_System * sys);
    PS_SystemVisitor GetSystems();

protected:
    String                      m_sName;
    String                      m_sSource;

    float                       m_lastUpdateTime;
    Camera *                    mCurrentCam;

    PS_SystemList               m_systems;
};

}