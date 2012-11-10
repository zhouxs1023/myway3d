#pragma once
#ifndef DEV_DATAPOOL_H
#define DEV_DATAPOOL_H

#include "Types.h"
#include "Memory.h"

namespace devil
{

#define INVALID_INDEX  0xFFFF

template <class T>
class MemberPool
{
public:
				MemberPool();
				~MemberPool();

	void		Create(short iMaxCount);
	void		Destroy();

	short		NextMember();
	short		AddMember(const T & Mem);
	void		Release(short index);

	bool		IsOpen(short index) const;
	
	short		GetTotalOpen() const;
	short		GetTotalUsed() const;
	short		GetFirstOpen() const;
	short		GetMaxCount() const;

	T &			Member(short index);
	const T &	Member(short index) const;
	T *			MemberPtr(short index);
	const T *	MemberPtr(short index) const;

private:
	short		mTotalOpen;
	short		mFirstOpen;
	short		mMaxCount;
	short *	mNextOpenList;
	T *			mMemberList;
};

template<class T>
inline MemberPool<T>::MemberPool()
: mTotalOpen(0), mFirstOpen(0), mMaxCount(0), mNextOpenList(0), mMemberList(0)
{
}

template<class T>
inline void MemberPool<T>::Create(short iMaxCount)
{
	debug_assert(!mNextOpenList && !mMemberList, "Group has already been created!");
    mMaxCount = iMaxCount;
	mNextOpenList = new short[mMaxCount];
	mMemberList = new T[mMaxCount];
	debug_assert(mNextOpenList && mMemberList, "Memory allocation error");

	for (int16 i = 0; i < mMaxCount; i++)
		mNextOpenList[i] = i + 1;
	mNextOpenList[mMaxCount - 1] = mMaxCount - 1;

	mTotalOpen = mMaxCount;
	mFirstOpen = 0;
}

template<class T>
inline void MemberPool<T>::Destroy()
{
	debug_assert(mMemberList && mNextOpenList, "Group has not been created!");
	debug_assert(mTotalOpen == mMaxCount, "Group is not empty");

	safe_delete_array(mNextOpenList);
	safe_delete_array(mMemberList);
	mTotalOpen = 0;
	mFirstOpen = 0;
}

template<class T>
inline short MemberPool<T>::NextMember()
{
	debug_assert(mMemberList && mNextOpenList, "Group has not been created!");
	debug_assert(mTotalOpen, "No Open");

	short index = mFirstOpen;
	mFirstOpen = mNextOpenList[index];
	mTotalOpen--;

	debug_assert(mFirstOpen != INVALID_INDEX, "Invalid Open Index");
	debug_assert(IsOpen(index), "Invalid Index");

	mNextOpenList[index] = INVALID_INDEX;

	return index;
}

template<class T>
inline short MemberPool<T>::AddMember(const T & Mem)
{
	short index = NextMember();
	mMemberList[index] = Mem;
	return index;
}

template<class T>
inline void MemberPool<T>::Release(short index)
{
	debug_assert(mMemberList && mNextOpenList, "Group has not been created!");
	debug_assert(index < mMaxCount, "Invalid Index");

	debug_assert(!IsOpen(index), "Invalid Index");

	mNextOpenList[index] = mTotalOpen ? mFirstOpen : index;
	mFirstOpen = index;

	mTotalOpen++;
	mFirstOpen = index;
}

template<class T>
inline bool MemberPool<T>::IsOpen(short index) const
{
	debug_assert(mMemberList && mNextOpenList, "Group has not been created!");
	debug_assert(index < mMaxCount, "Invalid Index");

	return mNextOpenList[index] != INVALID_INDEX;
}

template<class T>
inline short MemberPool<T>::GetTotalOpen() const
{
	return mTotalOpen;
}

template<class T>
inline short MemberPool<T>::GetTotalUsed() const
{
	return mMaxCount - mTotalOpen
}

template<class T>
inline short MemberPool<T>::GetFirstOpen() const
{
	return mFirstOpen;
}

template<class T>
inline short MemberPool<T>::GetMaxCount() const
{
	return mMaxCount;
}

template<class T>
T &	MemberPool<T>::Member(short index)
{
	debug_assert(mMemberList && mNextOpenList, "Group has not been created!");
	debug_assert(index < mMaxCount, "Invalid Index");
	return mMemberList[index];
}

template<class T>
const T & MemberPool<T>::Member(short index) const
{
	debug_assert(mMemberList && mNextOpenList, "Group has not been created!");
	debug_assert(index < mMaxCount, "Invalid Index");
	return mMemberList[index];
}

template<class T>
T *	MemberPool<T>::MemberPtr(short index)
{
	debug_assert(mMemberList && mNextOpenList, "Group has not been created!");
	debug_assert(index < mMaxCount, "Invalid Index");
	return &mMemberList[index];
}

template<class T>
const T * MemberPool<T>::MemberPtr(short index) const
{
	debug_assert(mMemberList && mNextOpenList, "Group has not been created!");
	debug_assert(index < mMaxCount, "Invalid Index");
	return &mMemberList[index];
}


}

#endif