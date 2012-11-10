#pragma once

#include "MWList.h"


namespace Myway
{

class Event;

class MW_ENTRY EventListener
{
    friend class Event;

public:
    EventListener();
    virtual ~EventListener();

    virtual void OnCall(Event * sender, void * data) {}

protected:
    EventListener * prev;
    EventListener * next;
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
    EventListener * head;
};

}
