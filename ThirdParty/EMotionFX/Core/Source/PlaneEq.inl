/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "PlaneEq.h"


template <class T>
bool TPlaneEq<T>::Clip(const Array< TVector3<T> >& pointsIn, Array< TVector3<T> >& pointsOut) const
{
	int	numPoints = pointsIn.GetLength();

	assert(&pointsIn != &pointsOut);
	assert(numPoints >= 2);

	int		vert1		= numPoints - 1;
	T		firstDist	= GetDistanceTo(pointsIn[vert1]);
	T		nextDist	= firstDist;
	bool	firstIn		= (firstDist >= 0.0);
	bool	nextIn		= firstIn;

	pointsOut.Clear();

	if (numPoints == 2) 
		numPoints = 1;

	for (int vert2=0; vert2<numPoints; vert2++)
	{
		T		dist = nextDist;
		bool	in	 = nextIn;

		nextDist = GetDistanceTo( pointsIn[vert2] );
		nextIn	 = (nextDist >= 0.0);

		if (in)
			pointsOut.Add( pointsIn[vert1] );

		if ((in != nextIn) && (dist!=0.0) && (nextDist!=0.0))
		{
			TVector3<T> dir = (pointsIn[vert2] - pointsIn[vert1]);
			
			T frac = dist / (dist - nextDist);
			if ((frac>0.0) && (frac<1.0))
				pointsOut.Add(pointsIn[vert1] + frac * dir);
		}

		vert1 = vert2;
	}

	//if (numPoints == 1) 
	//	return (pointsOut.GetLength() > 1);

	return (pointsOut.GetLength() > 1);
}



// Clip a set of vectors <points> to this plane
template <class T>
bool TPlaneEq<T>::Clip(Array< TVector3<T> >& points) const
{
	Array< TVector3<T> > pointsOut;
	if (Clip(points, pointsOut))
	{
		points = pointsOut;
		return true;
	}

	return false;
}
