#pragma once

#include "MGUI_Types.h"

namespace Myway {

	template <class T, class P1, class P2, class P3, class P4, class P5, class P6>
	class _tListener6
	{
		friend typename T;

		_tListener6 * _Prev;
		_tListener6 * _Next;

	public:
		_tListener6() : mEvent(NULL) {}
		virtual ~_tListener6() { if (mEvent) (*mEvent) -= this; }

		virtual void OnCall(P1 _p1, P2 _p2, P3 _p3, P4 _p4, P5 _p5, P6 _p6) = 0;

	protected:
		T * mEvent;
	};

	template <class P1, class P2, class P3, class P4, class P5, class P6>
	class tEvent6
	{
	public:
		typedef _tListener6<tEvent6, P1, P2, P3, P4, P5, P6> tMyListener;

	public:
		tEvent6() { mHead = NULL; }
		~tEvent6() { while (mHead) *this -= mHead; }

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

		void operator ()(P1 _p1, P2 _p2, P3 _p3, P4 _p4, P5 _p5, P6 _p6)
		{
			tMyListener * _node = mHead;

			while (_node != NULL)
			{
				_node->OnCall(_p1, _p2, _p3, _p4, _p5, _p6);

				_node = _node->_Next;
			}
		}

	protected:
		tMyListener * mHead;
	};

	template <class T, class P1, class P2, class P3, class P4, class P5, class P6>
	class tListener6 : public _tListener6<tEvent6<P1, P2, P3, P4, P5, P6>, P1, P2, P3, P4, P5, P6>
	{
		typedef void (T::*Function)(P1 _param1, P2 _param2, P3 _param3, P4 _param4, P5 _param5, P6 _param6);

	public:
		tListener6() : mListener(NULL), mFunction(NULL) {}
		virtual ~tListener6() {}

		virtual void OnCall(P1 _p1, P2 _p2, P3 _p3, P4 _p4, P5 _p5, P6 _p6)
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)(_p1, _p2, _p3, _p4, _p5, _p6);
		}

		tListener0 * operator ()(T * _listener, Function _func)
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