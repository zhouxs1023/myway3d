#pragma once

#include "MGUI_Types.h"

namespace Myway {

	template <class T, class P1>
	class _tListener1
	{
		friend typename T;

		_tListener1 * _Prev;
		_tListener1 * _Next;

	public:
		_tListener1() : mEvent(NULL) {}
		virtual ~_tListener1() { if (mEvent) (*mEvent) -= this; }

		virtual void OnCall(P1 _p1) = 0;

	protected:
		T * mEvent;
	};

	template <class P1>
	class tEvent1
	{
	public:
		typedef _tListener1<tEvent1, P1> tMyListener;

	public:
		tEvent1() { mHead = NULL; }
		~tEvent1() { while (mHead) *this -= mHead; }

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

		void operator ()(P1 _p1)
		{
			tMyListener * _node = mHead;

			while (_node != NULL)
			{
				_node->OnCall(_p1);

				_node = _node->_Next;
			}
		}

	protected:
		tMyListener * mHead;
	};

	template <class T, class P1>
	class tListener1 : public _tListener1<tEvent1<P1>, P1>
	{
		typedef void (T::*Function)(P1 _param1);

	public:
		tListener1() : mListener(NULL), mFunction(NULL) {}
		virtual ~tListener1() {}

		virtual void OnCall(P1 _p1)
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)(_p1);
		}

		tListener1 * operator ()(T * _listener, Function _func)
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