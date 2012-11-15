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

template <class T>
class tEventListener : public EventListener
{
	typedef void (T::*F)(void *);
public:
	tEventListener(Event & e, T * p, F f) : listener(p), evt(e), func(f)
	{ 
		evt += this;
	}

	~tEventListener()
	{
		evt -= this;
	}

	virtual void OnCall(Event * sender, void * data)
	{
		if (sender == &evt)
			(listener->*func)(data);
	}

protected:
	T * listener;
	F func;
	Event & evt;
};

}
