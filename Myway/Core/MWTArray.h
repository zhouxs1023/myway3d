#pragma once

#include "MWAllocObj.h"

namespace Myway
{

	template <class T, int _Size>
	class TArray
	{
		DECLARE_ALLOC();

	public:
		TArray()
		{
			mSize = 0;
		}

		~TArray();

		int Size() const
		{
			return mSize;
		}

		int MaxSize() const
		{
			return _Size;
		}

		bool Empty() const
		{
			return mSize == 0;
		}

		void Clear()
		{
			mSize = 0;
		}

		void Insert(int index, const T & v)
		{
			d_assert (index >= 0 && index <= mSize && mSize < _Size);

			for (int i = mSize - 1; i >= index; --i)
			{
				mMember[i + 1] = mMember[i];
			}

			mMember[index] = v;

			++mSize;
		}

		void Erase(int index)
		{
			d_assert (index >= 0 && index < mSize);

			for (int i = index; i < mSize - 1; ++i)
			{
				mMember[i] = mMember[i + 1];
			}

			--mSize;
		}

		void PushFront(const T & v)
		{
			Insert(0, v);
		}

		void PopFront()
		{
			Erase(0);
		}

		void PushBack(const T & v)
		{
			Insert(mSize, v);
		}

		void PopBack()
		{
			Erase(mSize - 1);
		}

		T & operator [](int index)
		{
			d_assert (index >= 0 && index < mSize);

			return mMember[index];
		}

		const T & operator [](int index) const
		{
			d_assert (index >= 0 && index < mSize);

			return mMember[index];
		}

		TArray(const TArray & rk)
		{
			mSize = rk.mSize;

			for (int i = 0; i < mSize; ++i)
			{
				mMember[i] = rk.mMember[i];
			}
		}

		TArray & operator =(const TArray & vec)
		{
			mSize = rk.mSize;

			for (int i = 0; i < mSize; ++i)
			{
				mMember[i] = rk.mMember[i];
			}
		}


	protected:
		int          mSize;
		T			 mMember[_Size];
	};
}
