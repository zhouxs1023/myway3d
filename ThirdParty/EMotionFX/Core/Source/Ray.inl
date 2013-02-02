/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Ray.h"

// ray-boundingsphere
template <class T>
bool TRay<T>::Intersects(const TBoundingSphere<T>& s, TVector3<T> *intersectA, TVector3<T> *intersectB) const
{
	TVector3<T> rayOrg = mOrigin - s.GetCenter();	// ray in space of the sphere

	T b, c, t1, t2, delta;

	b = rayOrg.Dot(mDirection);
	c = rayOrg.SquareLength() - s.GetRadiusSquared();

	delta = ((b*b) - c);

	if (delta < 0)
		return false; // no intersection

	if (intersectA==NULL && intersectB==NULL) return true;

	if (delta != 0)
	{
		delta = Math::Sqrt(delta);

		t1 = (-b + delta); if (t1<0) return false;
		t2 = (-b - delta); if (t2<0) return false;

		if (t1<t2) 
		{
			if (intersectA) (*intersectA) = mOrigin + mDirection * t1;
			if (intersectB) (*intersectB) = mOrigin + mDirection * t2;
		}
		else
		{
			if (intersectA) (*intersectA) = mOrigin + mDirection * t2;
			if (intersectB) (*intersectB) = mOrigin + mDirection * t1;
		}
	}
	else 
	{
		// if we are here it means that delta equals zero and we have only one solution
		t1 = (-b);
		if (intersectA) (*intersectA) = mOrigin + mDirection * t1;
		if (intersectB) (*intersectB) = (*intersectA);
	}

	return true;
}



// ray-plane
template <class T>
bool TRay<T>::Intersects(const TPlaneEq<T>& p, TVector3<T> *intersect) const
{
	// check if ray is parallel to plane (no intersection) or ray pointing away from plane (no intersection)
	T dot1 = p.GetNormal().Dot(mDirection);
	//if (dot1 >= 0) return false;	// backface cull

	// calc second dot product
	T dot2 = -(p.GetNormal().Dot(mOrigin) + p.GetDist());

	// calc t value
	T t = dot2 / dot1;

	// if t<0 then the line defined by the ray, intersects the plane behind the rays origin and so no 
	// intersection occurs. else we can calculate the intersection point
	if (t<0) return false;

	// calc intersection point
	if (intersect)
	{
		intersect->x = mOrigin.x + (mDirection.x*t);
		intersect->y = mOrigin.y + (mDirection.y*t);
		intersect->z = mOrigin.z + (mDirection.z*t);
	}

	return true;
}



// ray-triangle
template <class T>
bool TRay<T>::Intersects(const TVector3<T>& p1, const TVector3<T>& p2, const TVector3<T>& p3, TVector3<T> *intersect, T *baryU, T *baryV) const
{
	TVector3<T>		edge1, edge2, edge3, tvec, pvec, qvec;
	T				det,inv_det;
	T				u,v;
	TVector3<T>		dir = mDest - mOrigin;

	// calculate two vectors of the polygon
	edge1 = p2 - p1;
	edge2 = p3 - p1;

	// begin calculating determinant - also used to calculate U parameter
	pvec = dir.Cross(edge2);

	// if determinant is near zero, ray lies in plane of triangle
	det = edge1.Dot(pvec);

	if (det > -Math::EPSILON && det < Math::EPSILON) return false;

	inv_det = 1.0 / det;

	// calculate distance from vert0 to ray origin
	tvec = mOrigin - p1;

	// calculate U parameter and test bounds
	u = tvec.Dot(pvec) * inv_det;
	if (u < 0.0 || u > 1.0)	return false;

	// prepare to test V parameter
	qvec = tvec.Cross(edge1);

	// calculate V parameter and test bounds
	v = dir.Dot(qvec) * inv_det;
	if (v < 0.0 || u + v > 1.0)	return false;

	// calculate t, ray intersects triangle
	float t = edge2.Dot(qvec) * inv_det;
	if (t<0.0 || t>1.0) return false;

	if (baryU)
		*baryU = u;

	if (baryV)
		*baryV = v;

	if (intersect)
		*intersect = mOrigin + t * dir;

	return true;
}




// ray-axis aligned bounding box
template <class T>
bool TRay<T>::Intersects(const TAABB<T>& b, TVector3<T> *intersectA, TVector3<T> *intersectB) const
{
	T tNear = -FLT_MAX, tFar=FLT_MAX;

	TVector3<T> minVec = b.GetMin();
	TVector3<T> maxVec = b.GetMax();


	// For all three axes, check the near and far intersection point on the two slabs
	for (int i=0; i<3; i++)
	{
		if (mDirection[i] == 0.0)
		{
			// Direction is parallel to this plane, check if we're somewhere between min and max
			if ((mOrigin[i] < minVec[i]) || (mOrigin[i] > maxVec[i])) return false;
		}
		else
		{
			// Calculate t's at the near and far slab, see if these are min or max t's
			T t1 = (minVec[i] - mOrigin[i]) / mDirection[i];
			T t2 = (maxVec[i] - mOrigin[i]) / mDirection[i];
			if (t1 > t2)
			{
				T temp = t1;
				t1 = t2;
				t2 = temp;
			}
			if (t1 > tNear) tNear = t1;							// Accept nearest value
			if (t2 < tFar)  tFar  = t2;							// Accept farthest value
			if ((tNear > tFar) || (tFar < 0.0))
				return false;
		}
	}

	if (intersectA)
	{
		*intersectA = mOrigin + /*(dest-origin)*/mDirection * tNear;
	}

	if (intersectB)
	{
		*intersectB = mOrigin + /*(dest-origin)*/mDirection * tFar;
	}

	return true;
}
