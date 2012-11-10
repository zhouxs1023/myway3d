#include "MWPSSystem.h"
#include "MWPSEmitter.h"
#include "MWPSAffector.h"
#include "MWPSRender.h"
#include "MWPSManager.h"
#include "MWParticleSystem.h"

using namespace Myway;

PS_System::PS_System(ParticleSystem * parent, const String & name)
:_myParent(parent),
 _myname(name),
 _myRender(NULL),
 _myQuota(0),
 _myEnable(FALSE)
{
}

PS_System::~PS_System()
{
    safe_delete (_myRender);

    EmitterList::Iterator whr0 = _myEmitters.Begin();
    EmitterList::Iterator end0 = _myEmitters.End();

    while (whr0 != end0)
    {
        delete *whr0;
        ++whr0;
    }

    AffectorList::Iterator whr1 = _myAffectors.Begin();
    AffectorList::Iterator end1 = _myAffectors.End();

    while (whr1 != end1)
    {
        delete *whr1;
        ++whr1;
    }
}

const String & PS_System::GetName() const
{
    return  _myname;
}

ParticleSystem * PS_System::GetParent() const
{
    return _myParent;
}

void PS_System::Update(float fElapsedTime)
{
    _Death(fElapsedTime);
    _Emit(fElapsedTime);
    _Affect(fElapsedTime);

    _myRender->Update();
}

PS_Render * PS_System::CreateRender(const String & name, const String & type)
{
    assert (!_myRender);

    _myRender = PS_Manager::Instance().CreateRender(name, type);
    _myRender->_myParent = this;

    return _myRender;
}

PS_Emitter * PS_System::CreateEmitter(const String & name, const String & type)
{
    assert (GetEmitter(name) == NULL);

    PS_Emitter * emitter = PS_Manager::Instance().CreateEmitter(name, type);

    if (emitter)
        _myEmitters.PushBack(emitter);

    return emitter;
}

PS_Affector * PS_System::CreateAffector(const String & name, const String & type)
{
    PS_Affector * affector = PS_Manager::Instance().CreateAffector(name, type);

    if (affector)
        _myAffectors.PushBack(affector);

    return affector;
}

PS_Emitter * PS_System::GetEmitter(const String & name)
{
    EmitterList::Iterator whr = _myEmitters.Begin();
    EmitterList::Iterator end = _myEmitters.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    return whr != end ? *whr : NULL;
}

PS_Affector * PS_System::GetAffector(const String & name)
{
    AffectorList::Iterator whr = _myAffectors.Begin();
    AffectorList::Iterator end = _myAffectors.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    return whr != end ? *whr : NULL;
}

void PS_System::DestroyEmitter(const String & name)
{
    EmitterList::Iterator whr = _myEmitters.Begin();
    EmitterList::Iterator end = _myEmitters.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    assert (whr != end);

    delete *whr;
    _myEmitters.Erase(whr);
}

void PS_System::DestroyAffector(const String & name)
{
    AffectorList::Iterator whr = _myAffectors.Begin();
    AffectorList::Iterator end = _myAffectors.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    assert (whr != end);

    delete *whr;
    _myAffectors.Erase(whr);
}

PS_Render * PS_System::GetRender()
{
    return _myRender;
}

PS_System::EmitterVisitor PS_System::GetEmitters()
{
    return EmitterVisitor(_myEmitters.Begin(), _myEmitters.End());
}

PS_System::AffectorVisitor PS_System::GetAffectors()
{
    return AffectorVisitor(_myAffectors.Begin(), _myAffectors.End());
}

PS_System::ParticleVisitor PS_System::GetParticles()
{
    return ParticleVisitor(_myParticles.Begin(), _myParticles.End());
}

int PS_System::GetParticleSize() const
{
    return _myParticles.Size();
}

void PS_System::_Death(float fElapsedTime)
{
    List<Particle*>::Iterator iter;
    iter = _myParticles.Begin();

    Particle * p;

    while (iter != _myParticles.End())
    {
        p = *iter;

        if (p->life < fElapsedTime)
        {
            iter = _myParticles.Erase(iter);
            _Free(p);
        }
        else
        {
            p->life -= fElapsedTime;
            ++iter;
        }
    }
}

void PS_System::_Affect(float fElapsedTime)
{
    AffectorVisitor v = GetAffectors();

    while (!v.Endof())
    {
        PS_Affector * affector = *v.Cursor();
        affector->_Affect(this, fElapsedTime);

        ++v;
    }
}

void PS_System::_Emit(float fElapsedTime)
{
    EmitterVisitor v = GetEmitters();

    while (!v.Endof())
    {
        PS_Emitter * emitter = *v.Cursor();
        emitter->_Update(fElapsedTime);

        if (emitter->GetEmitted())
        {
            int count = emitter->GetEmittionCount();

            for (int i = 0; i < count; ++i)
            {
                Particle * p = _Quest();
                if (p)
                {
                    emitter->_InitParticle(p);
                    _myParticles.PushBack(p);
                }
                else
                {
                    break;
                }
            }
        }

        ++v;
    }
}

void PS_System::AddRenderQueue(RenderQueue * rq)
{
    _myRender->AddRenderQueue(rq);
}

Particle * PS_System::_Quest()
{
    Particle * p = NULL;

    if (!_myFree.Empty())
    {
        p = _myFree.Top();
        _myFree.Pop();
    }

    return p;
}

void PS_System::_Free(Particle * p)
{
    _myFree.Push(p);
}

void PS_System::SetQuota(int quota)
{
    assert (_myRender);

    _myQuota = quota;

    _myPool.Resize(quota);
    for (int i = 0; i < quota; ++i)
    {
        _myFree.Push(&_myPool[i]);
    }

    _myRender->NotifyResizeQuota();
}

int PS_System::GetQuota() const
{
    return _myQuota;
}

void PS_System::SetEnable(bool enable)
{
    if (!enable && _myEnable && _myParticles.Size())
    {
        _myParticles.Clear();
        _myFree.Clear();
        for (int i = 0; i < _myQuota; ++i)
        {
            _myFree.Push(&_myPool[i]);
        }
    }

    _myEnable = enable;
}

bool PS_System::GetEnable() const
{
    return _myEnable;
}