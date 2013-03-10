#include "MWEvent.h"

namespace Myway {

    EventListener::EventListener()
    {
        prev = next = NULL;
		evt = NULL;
    }

    EventListener::~EventListener()
    {
		if (evt)
			(*evt) -= this;
    }






    Event::Event()
		: head(NULL)
		, userData(NULL)
    {
		paramCount = 0;
    }

    Event::~Event()
    {
        d_assert (head == NULL);
    }

    void Event::Call()
    {
        EventListener * el = head;

        while (el)
        {
            el->OnCall(this);

            el = el->next;
        }
    }

	void Event::operator ()(void)
	{
		paramCount = 0;

		Call();
	}

	void Event::operator ()(void * p0)
	{
		paramCount = 1;
		param[0] = p0;

		Call();
	}

	void Event::operator ()(void * p0, void * p1)
	{
		paramCount = 2;
		param[0] = p0;
		param[1] = p1;

		Call();
	}

	void Event::operator ()(void * p0, void * p1, void * p2)
	{
		paramCount = 3;
		param[0] = p0;
		param[1] = p1;
		param[2] = p2;

		Call();
	}

	void Event::operator ()(void * p0, void * p1, void * p2, void * p3)
	{
		paramCount = 4;
		param[0] = p0;
		param[1] = p1;
		param[2] = p2;
		param[3] = p3;

		Call();
	}

	void Event::operator ()(void * p0, void * p1, void * p2, void * p3, void * p4)
	{
		paramCount = 5;
		param[0] = p0;
		param[1] = p1;
		param[2] = p2;
		param[3] = p3;
		param[4] = p4;

		Call();
	}

	void Event::operator ()(void * p0, void * p1, void * p2, void * p3, void * p4, void * p5)
	{
		paramCount = 6;
		param[0] = p0;
		param[1] = p1;
		param[2] = p2;
		param[3] = p3;
		param[4] = p4;
		param[5] = p5;

		Call();
	}

	void Event::operator ()(void * p0, void * p1, void * p2, void * p3, void * p4, void * p5, void * p6)
	{
		paramCount = 7;
		param[0] = p0;
		param[1] = p1;
		param[2] = p2;
		param[3] = p3;
		param[4] = p4;
		param[5] = p5;
		param[6] = p6;

		Call();
	}

	void Event::operator ()(void * p0, void * p1, void * p2, void * p3, void * p4, void * p5, void * p6 ,void * p7)
	{
		paramCount = 8;
		param[0] = p0;
		param[1] = p1;
		param[2] = p2;
		param[3] = p3;
		param[4] = p4;
		param[5] = p5;
		param[6] = p6;
		param[7] = p7;

		Call();
	}

    void Event::operator += (EventListener * p)
    {
        d_assert (p->prev == NULL && p->next == NULL && p->evt == NULL);

        if (head)
            head->prev = p;

        p->next = head;
        head = p;

		p->evt = this;
    }

    void Event::operator -= (EventListener * p)
    {
		d_assert (p->evt && p->evt == this);

        if (head == p)
        {
            head = p->next;
        }
        else
        {
            if (p->prev)
                p->prev->next = p->next;

            if (p->next)
                p->next->prev = p->prev;
        }

        p->prev = p->next = NULL;
		p->evt = NULL;
    }
}