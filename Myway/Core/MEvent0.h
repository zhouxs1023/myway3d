#pragma once

#include "MWDebug.h"

namespace Myway {

	template <class T>
	class _tListener0
	{
		friend typename T;

		_tListener0 * _Prev;
		_tListener0 * _Next;

	public:
		_tListener0() : mEvent(NULL), _Prev(NULL), _Next(NULL) {}
		virtual ~_tListener0() { if (mEvent) (*mEvent) -= this; }

		virtual void OnCall() = 0;

	protected:
		T * mEvent;
	};

	class tEvent0
	{
	public:
		typedef _tListener0<tEvent0> tMyListener;

	public:
		tEvent0() { mHead = NULL; }
		~tEvent0() { while (mHead) *this -= mHead; }

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

		void operator ()()
		{
			tMyListener * _node = mHead;

			while (_node != NULL)
			{
				_node->OnCall();

				_node = _node->_Next;
			}
		}

	protected:
		tMyListener * mHead;
	};

	template <class T>
	class tListener0 : public _tListener0<tEvent0>
	{
		typedef void (T::*Function)();

	public:
		tListener0() : mListener(NULL), mFunction(NULL) {}
		virtual ~tListener0() {}

		virtual void OnCall()
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)();
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