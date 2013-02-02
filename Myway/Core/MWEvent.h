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

    virtual void OnCall(Event * sender, void * param0, void * param1) = 0;

protected:
    EventListener * prev;
    EventListener * next;
};

class MW_ENTRY Event
{
public:
    Event();
    ~Event();

    void Call(void * param0, void * param1);

    void operator += (EventListener * p);
    void operator -= (EventListener * p);
	void operator ()(void * param0, void * param1) { Call(param0, param1); }

protected:
    EventListener * head;
};

template <class T>
class tEventListener : public EventListener
{
	typedef void (T::*F)(void * param0, void * param1);
public:
	tEventListener(T * p, F f) : listener(p), func(f)
	{
		evt = NULL;
	}

	tEventListener(Event * e, T * p, F f) : listener(p), func(f)
	{
		evt = NULL;
		Attach(e);
	}

	~tEventListener()
	{
		Detach();
	}

	void Attach(Event * e)
	{
		d_assert (e != NULL);

		if (evt == e)
			return ;

		Detach();

		evt = e;

		(*evt) += this;
	}

	void Detach()
	{
		if (evt)
			(*evt) -= this;

		evt = NULL;
	}

	virtual void OnCall(Event * sender, void * param0, void * param1)
	{
		if (sender == evt)
			(listener->*func)(param0, param1);
	}

protected:
	Event * evt;
	T * listener;
	F func;
};

}
