#pragma once

#include "MGUI_Types.h"

namespace Myway {

	template <class T, class P1, class P2>
	class _tListener2
	{
		friend typename T;

		_tListener2 * _Prev;
		_tListener2 * _Next;

	public:
		_tListener2() : mEvent(NULL) {}
		virtual ~_tListener2() { if (mEvent) (*mEvent) -= this; }

		virtual void OnCall(P1 _p1, P2 _p2) = 0;

	protected:
		T * mEvent;
	};

	template <class P1, class P2>
	class tEvent2
	{
	public:
		typedef _tListener2<tEvent2, P1, P2> tMyListener;

	public:
		tEvent2() { mHead = NULL; }
		~tEvent2() { while (mHead) *this -= mHead; }

		void operator +=(tMyListener * _listener)
		{
			d_assert (_listener != NULL && _listener->mEvent == NULL);
			d_assert (_listener->_Prev == NULL && _listener->_Next == NULL);

			if (mHead)
				mHead->_Prev = _listener;

			_listener->_Next = mHead;
			mHead = _listener;
			mHead->_Prev = NULL;

			_listener->mEvent = this;
		}

		void operator -=(tMyListener * _listener)
		{
			d_assert (_listener != NULL && _listener->mEvent == this);
			d_assert (mHead != NULL);

			if (_listener == mHead)
				mHead = _listener->_Next;

			if (_listener->_Prev)
				_listener->_Prev->_Next = _listener->_Next;

			if (_listener->_Next)
				_listener->_Next->_Prev = _listener->_Prev;

			_listener->_Prev = _listener->_Next = NULL;
			_listener->mEvent = NULL;
		}

		void operator ()(P1 _p1, P2 _p2)
		{
			tMyListener * _node = mHead;

			while (_node != NULL)
			{
				_node->OnCall(_p1, _p2);

				_node = _node->_Next;
			}
		}

	protected:
		tMyListener * mHead;
	};

	template <class T, class P1, class P2>
	class tListener2 : public _tListener2<tEvent2<P1, P2>, P1, P2>
	{
		typedef void (T::*Function)(P1 _param1, P2 _param2);

	public:
		tListener2() : mListener(NULL), mFunction(NULL) {}
		virtual ~tListener2() {}

		virtual void OnCall(P1 _p1, P2 _p2)
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)(_p1, _p2);
		}

		tListener2 * operator ()(T * _listener, Function _func)
		{
			mListener = _listener;
			mFunction = _func;
			return this;
		}

	protected:
		T * mListener;
		Function mFunction;
	};
}