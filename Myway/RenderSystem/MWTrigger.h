#pragma once

#include "MWTimer.h"
#include "MWSystem.h"
#include "MWHashMap.h"
#include "MWSingleton.h"

namespace Myway
{

class Trigger;

typedef void (*TriggerFunc)(Trigger * trigger);

struct TriggerSlot : public AllocObj
{
public:
    TriggerSlot() {}
    virtual ~TriggerSlot() {}

    virtual void operator ()(Trigger * trigger) = 0;
};

template <class T>
struct TriggerMemberSlot : public TriggerSlot
{
public:
    TriggerMemberSlot(TriggerFunc func, T * obj) : munc(func), m_obj(obj) {}
    virtual ~TriggerMemberSlot() {}

    virtual void operator()(Trigger * trigger) { m_obj->(*munc)(trigger); }

protected:
    TriggerFunc munc;
    T * m_obj;
};

struct TriggerGlobalSlot : public TriggerSlot
{
public:
    TriggerGlobalSlot(TriggerFunc func) : munc(func) {}
    virtual ~TriggerGlobalSlot() {}

    virtual void operator()(Trigger * trigger) { (*munc)(trigger); }

protected:
    TriggerFunc munc;
};


class MW_ENTRY Trigger : public AllocObj
{
    friend class TriggerMgr;

public:
    Trigger(int id, float time, TriggerSlot * slot) : md(id), m_time(time), m_slot(slot) {}
    ~Trigger() { delete m_slot; }

    int GetId() const { return md; }
    float GetTime() const { return m_time; }
    float GetIntervalTime() const { return mntervalTime; }

protected:
    void _Call() { m_slot->operator ()(this); }

protected:
    int      md;
    float       m_time;
    TriggerSlot *  m_slot;

    float       mnternalTime;
    float       mntervalTime;
};

class MW_ENTRY TriggerMgr
{
    DECLARE_SINGLETON(TriggerMgr);

    typedef HashMap<int, Trigger*> TriggerMap;

public:
    TriggerMgr();
    ~TriggerMgr();

    void Update();

    template <class T>
    void OnTrigger(int id, float time, TriggerFunc func, T * obj)
    {
        TriggerMemberSlot * slot = new TriggerMemberSlot(func, obj);
        Trigger * trigger = new Trigger(id, time, slot);

        float curtime = System::InternalTime.ElapsedTime();
        trigger->mnternalTime = curtime;
        trigger->mntervalTime = 0;

        mMTriggers.Insert(id, trigger);
    }

    void OnTrigger(int id, float time, TriggerFunc func)
    {
        TriggerGlobalSlot * slot = new TriggerGlobalSlot(func);
        Trigger * trigger = new Trigger(id, time, slot);

        float curtime = System::InternalTime.ElapsedTime();
        trigger->mnternalTime = curtime;
        trigger->mntervalTime = 0;

        mMTriggers.Insert(id, trigger);
    }

    void KillTrigger(int id)
    {
        TriggerMap::Iterator whr = mMTriggers.Find(id);
        TriggerMap::Iterator end = mMTriggers.End();

        if (whr != end)
        {
            delete whr->second;
            mMTriggers.Erase(whr);
        }
    }

protected:
    TriggerMap  mMTriggers;
};

}