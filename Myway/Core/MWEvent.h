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

    virtual void OnCall(Event * sender) = 0;

protected:
    EventListener * prev;
    EventListener * next;

	Event * evt;
};

class MW_ENTRY Event
{
	static const int MAX_PARAM_COUNT = 8;
public:
    Event();
    ~Event();

	int GetParamCount() const { return paramCount; }
	void * GetParam(int index) const { d_assert (index < paramCount); return param[index]; }

    void operator += (EventListener * p);
    void operator -= (EventListener * p);

	void operator ()(void);
	void operator ()(void * p0);
	void operator ()(void * p0, void * p1);
	void operator ()(void * p0, void * p1, void * p2);
	void operator ()(void * p0, void * p1, void * p2, void * p3);
	void operator ()(void * p0, void * p1, void * p2, void * p3, void * p4);
	void operator ()(void * p0, void * p1, void * p2, void * p3, void * p4, void * p5);
	void operator ()(void * p0, void * p1, void * p2, void * p3, void * p4, void * p5, void * p6);
	void operator ()(void * p0, void * p1, void * p2, void * p3, void * p4, void * p5, void * p6 ,void * p7);

private:
	void Call();

protected:
	void * userData;
    EventListener * head;

	int paramCount;
	void * param[MAX_PARAM_COUNT];
};

template <class T>
class tEventListener : public EventListener
{
	typedef void (T::*F)(Event * sender);

public:
	tEventListener(T * p, F f) : listener(p), func(f)
	{
	}

	tEventListener(Event & e, T * p, F f) : listener(p), func(f)
	{
		e += this;
	}

	~tEventListener()
	{
	}

	virtual void OnCall(Event * sender)
	{
		if (sender == evt)
			(listener->*func)(sender);
	}

protected:
	T * listener;
	F func;
};

}
