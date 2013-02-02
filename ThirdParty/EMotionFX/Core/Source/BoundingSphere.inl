/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "BoundingSphere.h"


template <class T>
void TBoundingSphere<T>::Encapsulate(const TVector3<T>& v)
{
	TVector3<T> diff = v - mCenter;
	T			dist = diff.Dot(diff);

	if (dist > mRadiusSq)
	{
		TVector3<T> diff2	= diff.Normalized() * mRadius;
		TVector3<T> delta	= 0.5 * (diff - diff2);
		mCenter				+= delta;
		mRadius				+= delta.Length();
		mRadiusSq			= mRadius*mRadius;
	}
}



template <class T>
bool TBoundingSphere<T>::Intersects(const TAABB<T>& b) const
{
	T distance = 0;

	for (int t=0; t<3; t++)
	{
		TVector3<T> minVec = b.GetMin();
		if (mCenter[t] < minVec[t]) 
		{
			distance += (mCenter[t] - minVec[t]) * (mCenter[t] - minVec[t]);
			if (distance > mRadiusSq) return false;
		}
		else
		{
			TVector3<T>	maxVec = b.GetMax();
			if (mCenter[t] > maxVec[t])
			{
				distance += (mCenter[t] - maxVec[t]) * (mCenter[t] - maxVec[t]);
				if (distance > mRadiusSq) return false;
			}
		}
	}

	return true;
}



template <class T>
bool TBoundingSphere<T>::Contains(const TAABB<T>& b) const
{
	T distance = 0;

	for (int t=0; t<3; t++)
	{
		TVector3<T> maxVec = b.GetMax();

		if (mCenter[t] < maxVec[t]) 
			distance += (mCenter[t] - maxVec[t]) * (mCenter[t] - maxVec[t]);
		else
		{
			TVector3<T> minVec = b.GetMin();

			if (mCenter[t] > minVec[t])
				distance += (mCenter[t] - minVec[t]) * (mCenter[t] - minVec[t]);
		}

		if (distance > mRadiusSq) return false;
	}

	return true;
}
