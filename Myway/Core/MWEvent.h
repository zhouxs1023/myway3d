#pragma once

#include "MWList.h"


namespace Myway
{

class Event;

class MW_ENTRY EventListener
{
public:
    EventListener();
    virtual ~EventListener();

    virtual void OnCall(Event * sender, void * data) {}
};

class MW_ENTRY Event
{
public:
    Event();
    ~Event();

    void Call(void * data = NULL);

    void operator += (EventListener * p);
    void operator -= (EventListener * p);

protected:
    List<EventListener*> mListeners;
};

}
