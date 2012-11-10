#include "MWEvent.h"

namespace Myway {

    EventListener::EventListener()
    {
    }

    EventListener::~EventListener()
    {
    }






    Event::Event()
    {
    }

    Event::~Event()
    {
        d_assert (mListeners.Size() == 0);
    }

    void Event::Call(void * data)
    {
        List<EventListener *>::Iterator whr = mListeners.Begin();
        List<EventListener *>::Iterator end = mListeners.End();

        while (whr != end)
        {
            (*whr)->OnCall(this, data);
            ++whr;
        }
    }

    void Event::operator += (EventListener * p)
    {
        mListeners.PushBack(p);
    }

    void Event::operator -= (EventListener * p)
    {
        List<EventListener *>::Iterator whr = mListeners.Begin();
        List<EventListener *>::Iterator end = mListeners.End();

        while (whr != end && (*whr) != p)
        {
            ++whr;
        }

        d_assert (whr != end);

        mListeners.Erase(whr);
    }
}