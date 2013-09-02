#pragma once

#include "MWDebug.h"

namespace Myway {

	template <class T, class P1, class P2, class P3>
	class _tListener3
	{
		friend typename T;

		_tListener3 * _Prev;
		_tListener3 * _Next;

	public:
		_tListener3() : mEvent(NULL), _Prev(NULL), _Next(NULL) {}
		virtual ~_tListener3() { if (mEvent) (*mEvent) -= this; }

		virtual void OnCall(P1 _p1, P2 _p2, P3 _p3) = 0;

	protected:
		T * mEvent;
	};

	template <class P1, class P2, class P3>
	class tEvent3
	{
	public:
		typedef _tListener3<tEvent3, P1, P2, P3> tMyListener;

	public:
		tEvent3() { mHead = NULL; }
		~tEvent3() { while (mHead) *this -= mHead; }

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

		void operator ()(P1 _p1, P2 _p2, P3 _p3)
		{
			tMyListener * _node = mHead;

			while (_node != NULL)
			{
				_node->OnCall(_p1, _p2, _p3);

				_node = _node->_Next;
			}
		}

	protected:
		tMyListener * mHead;
	};

	template <class T, class P1, class P2, class P3>
	class tListener3 : public _tListener3<tEvent3<P1, P2, P3>, P1, P2, P3>
	{
		typedef void (T::*Function)(P1 _param1, P2 _param2, P3 _param3);

	public:
		tListener3() : mListener(NULL), mFunction(NULL) {}
		virtual ~tListener3() {}

		virtual void OnCall(P1 _p1, P2 _p2, P3 _p3)
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)(_p1, _p2, _p3);
		}

		tListener3 * operator ()(T * _listener, Function _func)
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