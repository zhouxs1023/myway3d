#include "MWEvent.h"

namespace Myway {

    EventListener::EventListener()
    {
        prev = next = NULL;
    }

    EventListener::~EventListener()
    {
        prev = next = NULL;
    }






    Event::Event()
    {
        head = NULL;
    }

    Event::~Event()
    {
        d_assert (head == NULL);
    }

    void Event::Call(void * data)
    {
        EventListener * el = head;

        while (el)
        {
            el->OnCall(this, data);

            el = el->next;
        }
    }

    void Event::operator += (EventListener * p)
    {
        d_assert (p->prev == NULL && p->next == NULL);

        if (head)
            head->prev = p;

        p->next = head;
        head = p;
    }

    void Event::operator -= (EventListener * p)
    {
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
    }
}