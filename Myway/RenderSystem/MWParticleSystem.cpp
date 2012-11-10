#include "MWParticleSystem.h"
#include "MWPSSystem.h"
#include "MWSceneManager.h"
#include "MWNode.h"

using namespace Myway;

DECLARE_GUID128_CONST(ParticleSystem::Guid, 'p', 'a', 'r', 't', 'i', 'c', 'l', 'e',
                                            's', 'y', 's', 't', 'e', 'm', 0, 0);

ParticleSystem::ParticleSystem(const String & sName)
: m_sName(sName),
  m_lastUpdateTime(0),
  mCurrentCam(NULL)
{
}

ParticleSystem::~ParticleSystem()
{
    PS_SystemList::Iterator whr = m_systems.Begin();
    PS_SystemList::Iterator end = m_systems.End();

    while (whr != end)
    {
        delete *whr;
        ++whr;
    }
}

const String & ParticleSystem::GetName() const
{
    return m_sName;
}

void ParticleSystem::GetWorldPosition(Vec3 * pos)
{
    *pos = m_node->GetWorldPosition();
}

void ParticleSystem::GetWorldTransform(Mat4 * form)
{
    *form = m_node->GetWorldMatrix();
}

LightList * ParticleSystem::GetLightList()
{
    return &m_lights;
}

const guid128 & ParticleSystem::GetGuid()
{
    return Guid;
}

void ParticleSystem::NotifyCamera(Camera * cam)
{
    mCurrentCam = cam;
}

Camera * ParticleSystem::GetCurrentCamera()
{
    return mCurrentCam;
}

void ParticleSystem::UpdateMover()
{
    float time = System::InternalTime.ElapsedTime();
    float elapsedTime = time - m_lastUpdateTime;
    elapsedTime = Math::Clamp(elapsedTime, 0.0f, 0.1f);

    m_lastUpdateTime = time;

    PS_SystemVisitor v = GetSystems();

    while (!v.Endof())
    {
        PS_System * system = *v.Cursor();
        system->Update(elapsedTime);

        ++v;
    }
}

void ParticleSystem::AddRenderQueue(RenderQueue * rq)
{
    PS_SystemVisitor v = GetSystems();

    while (!v.Endof())
    {
        PS_System * system = *v.Cursor();
        system->AddRenderQueue(rq);

        ++v;
    }
}

PS_System * ParticleSystem::CreateSystem(const String & name)
{
    PS_System * system = new PS_System(this, name);

    m_systems.PushBack(system);

    return system;
}

PS_System * ParticleSystem::GetSystem(const String & name)
{
    PS_SystemList::Iterator whr = m_systems.Begin();
    PS_SystemList::Iterator end = m_systems.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    assert (whr != end);

    return *whr;
}

void ParticleSystem::RemoveSystem(const String & name)
{
    PS_SystemList::Iterator whr = m_systems.Begin();
    PS_SystemList::Iterator end = m_systems.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    assert (whr != end);

    delete *whr;
    m_systems.Erase(whr);
}

void ParticleSystem::RemoveSystem(PS_System * sys)
{
    RemoveSystem(sys->GetName());
}

ParticleSystem::PS_SystemVisitor ParticleSystem::GetSystems()
{
    return PS_SystemVisitor(m_systems.Begin(), m_systems.End());
}