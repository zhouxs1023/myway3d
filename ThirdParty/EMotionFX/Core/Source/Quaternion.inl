/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Quaternion.h"


// multiply a vector by a quaternion
template <class T>
TVector3<T>	TQuaternion<T>::operator * (const TVector3<T>& p)
{
	TQuaternion<T> v(p.x, p.y, p.z, 0.0);
	v = *this * v * this->Conjugated();
	return TVector3<T>(v.x, v.y, v.z);
}



// returns the ratio of two quaternions
template <class T>
TQuaternion<T> TQuaternion<T>::operator / (const TQuaternion<T>& q)
{
	TQuaternion<T> t((*this) * -q);
	TQuaternion<T> s((-q) * (-q));
	t *= (1.0 / s.w);
	return t;
}




// spherical quadratic interpolation
template <class T>
TQuaternion<T> TQuaternion<T>::Squad(const TQuaternion<T>& p, const TQuaternion<T>& a, const TQuaternion<T>& b, const TQuaternion<T>& q, const T t)
{
	TQuaternion<T> q0(p.Slerp(q, t));
	TQuaternion<T> q1(a.Slerp(b, t));
	return q0.Slerp(q1, 2 * t * (1 - t));
}



// returns the spherical interpolated result [t must be between 0..1]
template <class T>
TQuaternion<T> TQuaternion<T>::Slerp(const TQuaternion<T>& to, const T t) const
{
	T cosom = (x*to.x) + (y*to.y) + (z*to.z) + (w*to.w);
	T scale0, scale1, scale1sign=1.0;

	if (cosom < 0)
	{	
		scale1sign	 = -1.0; 
		cosom		*= -1.0;
	}
	
	if ((1.0 - cosom) > Math::EPSILON)
	{		
		T omega		= Math::ACos(cosom);
		T sinOmega  = Math::Sin(omega);
		T oosinom	= 1.0 / sinOmega;
		scale0		= Math::Sin((1.0 - t) * omega) * oosinom;
		scale1		= Math::Sin(t * omega) * oosinom;
	}
	else
	{
		scale0 = 1.0 - t;
		scale1 = t;
	}

	scale1 *= scale1sign;

	return TQuaternion<T>(	scale0*x + scale1*to.x,
							scale0*y + scale1*to.y,
							scale0*z + scale1*to.z,
							scale0*w + scale1*to.w);
}




// returns the linear interpolated result [t must be between 0..1]
template <class T>
TQuaternion<T> TQuaternion<T>::Lerp(const TQuaternion<T>& to, T t) const
{
	T omt = 1.0 - t;
	T cosom = (x*to.x) + (y*to.y) + (z*to.z) + (w*to.w);
	
	if (cosom<0)
		t = -t;

	return TQuaternion<T>(	omt*x + t*to.x,
							omt*y + t*to.y,
							omt*z + t*to.z,
							omt*w + t*to.w);
}



// quaternion from an axis and angle
template <class T>
TQuaternion<T>::TQuaternion(const TVector3<T>& axis, const T angle)
{
	T squaredLength = axis.SquareLength();

	if (squaredLength > 0.0)
	{
		T halfAngle = angle * 0.5;
		T sinScale = Math::Sin(halfAngle) / Math::Sqrt(squaredLength);
		x = axis.x * sinScale;
		y = axis.y * sinScale;
		z = axis.z * sinScale;
		w = Math::Cos(halfAngle);
	}
	else
	{
		x=y=z=0;
		w=1;
	}
}



// quaternion from a spherical rotation
template <class T>
TQuaternion<T>::TQuaternion(const TVector2<T>& spherical, const T angle)
{
	T latitude  = spherical.x;
	T longitude = spherical.y;

	T s = Math::Sin(angle / 2.0);
	T c = Math::Cos(angle / 2.0);

	T sin_lat = Math::Sin(latitude);
	T cos_lat = Math::Cos(latitude);

	T sin_lon = Math::Sin(longitude);
	T cos_lon = Math::Cos(longitude);

	x = s * cos_lat * sin_lon;
	y = s * sin_lat;
	z = s * sin_lat * cos_lon;
	w = c;
}


// convert to an axis and angle
template <class T>
void TQuaternion<T>::ToAxisAngle(TVector3<T>* axis, T* angle) const
{
	*angle = 2.0 * Math::ACos(w);
	T s = Math::Sin(*angle * 0.5);
	axis->x = x / s;
	axis->y = y / s;
	axis->z = z / s;
}


//	converts from unit quaternion to spherical rotation angles
template <class T>
void TQuaternion<T>::ToSpherical(TVector2<T>* spherical, T* angle) const
{
	TVector3<T> axis;
	ToAxisAng(&axis, angle);

	T latitude = -Math::ASin(axis.y);
	T longitude;

	if (axis.x * axis.x + axis.z * axis.z < 0.0001)
		longitude = 0;
	else 
	{
		longitude = Math::ATan2(axis.x, axis.z);
		if(longitude < 0.0)
			longitude += Math::TWO_PI;
	}

	spherical->x = latitude;
	spherical->y = longitude;
}



// setup the quaternion from a roll, pitch and yaw
template <class T>
TQuaternion<T>& TQuaternion<T>::SetEuler(const T pitch, const T yaw, const T roll)
{
	// METHOD #1:
	const T halfYaw		= yaw   * 0.5;
	const T halfPitch	= pitch * 0.5;
	const T halfRoll	= roll  * 0.5;

	const T cY = Math::Cos( halfYaw );
	const T sY = Math::Sin( halfYaw );
	const T cP = Math::Cos( halfPitch );
	const T sP = Math::Sin( halfPitch );
	const T cR = Math::Cos( halfRoll );
	const T sR = Math::Sin( halfRoll );

	x = cY * sP * cR - sY * cP * sR;
	y = cY * sP * sR + sY * cP * cR;
	z = cY * cP * sR - sY * sP * cR;
	w = cY * cP * cR + sY * sP * sR;

//	Normalize();	// we might be able to leave the normalize away, but better safe than not, this is more robust :)

	return *this;

/*

	// METHOD #2:
	TQuaternion<T> Qx(Vector3(sP, 0, 0), cP);
	TQuaternion<T> Qy(Vector3(0, sY, 0), cY);
	TQuaternion<T> Qz(Vector3(0, 0, sR), cR);

	TQuaternion<T> result = Qx * Qy * Qz;

	x = result.x;
	y = result.y;
	z = result.z;
	w = result.w;

	return *this;
*/
}



// convert the quaternion to a matrix
template <class T>
TMatrix<T> TQuaternion<T>::ToMatrix() const
{
	TMatrix<T> m(true);

    T xx=x*x;
    T xy=x*y, yy=y*y;
    T xz=x*z, yz=y*z, zz=z*z;
    T xw=x*w, yw=y*w, zw=z*w, ww=w*w;

	m.m44[0][0] = +xx-yy-zz+ww;			m.m44[0][1] = +xy+zw+xy+zw;		m.m44[0][2] = +xz-yw+xz-yw;
	m.m44[1][0] = +xy-zw+xy-zw;			m.m44[1][1] = -xx+yy-zz+ww;		m.m44[1][2] = +yz+xw+yz+xw;
	m.m44[2][0] = +xz+yw+xz+yw;			m.m44[2][1] = +yz-xw+yz-xw;		m.m44[2][2] = -xx-yy+zz+ww;
	
	return m;
}



// construct the quaternion from a given rotation matrix
template <class T>
TQuaternion<T> TQuaternion<T>::FromMatrix(const TMatrix<T>& m)
{
	TQuaternion<T> result;

	T trace = m.m44[0][0] + m.m44[1][1] + m.m44[2][2] + 1.0;
	if (trace > Math::EPSILON )
	{
		T s = 0.5 / Math::Sqrt(trace);
		result.w = 0.25 / s;
		result.x = ( m.m44[1][2] - m.m44[2][1] ) * s;
		result.y = ( m.m44[2][0] - m.m44[0][2] ) * s;
		result.z = ( m.m44[0][1] - m.m44[1][0] ) * s;
	} 
	else
	{
		if (m.m44[0][0] > m.m44[1][1] && m.m44[0][0] > m.m44[2][2])
		{
			T s = 2.0 * Math::Sqrt( 1.0 + m.m44[0][0] - m.m44[1][1] - m.m44[2][2]);
			T oneOverS = 1.0 / s;
			result.x = 0.25 * s;
			result.y = (m.m44[1][0] + m.m44[0][1] ) * oneOverS;
			result.z = (m.m44[2][0] + m.m44[0][2] ) * oneOverS;
			result.w = (m.m44[2][1] - m.m44[1][2] ) * oneOverS;
		}
		else 
		if (m.m44[1][1] > m.m44[2][2]) 
		{
			T s = 2.0 * Math::Sqrt( 1.0f + m.m44[1][1] - m.m44[0][0] - m.m44[2][2]);
			T oneOverS = 1.0 / s;
			result.x = (m.m44[1][0] + m.m44[0][1] ) * oneOverS;
			result.y = 0.25 * s;
			result.z = (m.m44[2][1] + m.m44[1][2] ) * oneOverS;
			result.w = (m.m44[2][0] - m.m44[0][2] ) * oneOverS;
		} 
		else
		{
			T s = 2.0 * Math::Sqrt( 1.0f + m.m44[2][2] - m.m44[0][0] - m.m44[1][1] );
			T oneOverS = 1.0 / s;
			result.x = (m.m44[2][0] + m.m44[0][2] ) * oneOverS;
			result.y = (m.m44[2][1] + m.m44[1][2] ) * oneOverS;
			result.z = 0.25 * s;
			result.w = (m.m44[1][0] - m.m44[0][1] ) * oneOverS;
		}
	}

	return result;

/*
	TQuaternion<T> result;

	T w = 0.5 * Math::Sqrt(m.m44[0][0] + m.m44[1][1] + m.m44[2][2] + m.m44[3][3]);
	T s = 0.25 / w;

	result.x = (m.m44[1][2] - m.m44[2][1]) * s;
	result.y = (m.m44[2][0] - m.m44[0][2]) * s;
	result.z = (m.m44[0][1] - m.m44[1][0]) * s;
	result.w = w;

	return result;
*/
}



/*
   cy = sqrt(data00*data00 + data10*data10);
   if(cy > 16.0*FLT_EPSILON)
   {
      ang->x = -atan2(data21, data22);
      ang->y = -atan2(-data20, cy);
      ang->z = -atan2(data10, data00);
   }
   else
   {
      ang->x = -atan2(-data12, data11);
      ang->y = -atan2(-data20, cy);
      ang->z = 0.0;
   }
   break;
 */


// convert a quaternion to euler angles (in degrees)
template <class T>
TVector3<T> TQuaternion<T>::ToEuler() const
{
/*
	// METHOD #1:

	TVector3<T> euler;

	T matrix[3][3];
	T cx,sx;
	T cy,sy,yr;
	T cz,sz;

	matrix[0][0] = 1.0 - (2.0 * y * y) - (2.0 * z * z);
	matrix[1][0] = (2.0 * x * y) + (2.0 * w * z);
	matrix[2][0] = (2.0 * x * z) - (2.0 * w * y);
	matrix[2][1] = (2.0 * y * z) + (2.0 * w * x);
	matrix[2][2] = 1.0 - (2.0 * x * x) - (2.0 * y * y);

	sy = -matrix[2][0];
	cy = Math::Sqrt(1 - (sy * sy));
	yr = Math::ATan2(sy,cy);
	euler.y = yr;

	// avoid divide by zero only where y ~90 or ~270
	if (sy != 1.0 && sy != -1.0)	
	{
		cx = matrix[2][2] / cy;
		sx = matrix[2][1] / cy;
		euler.x = Math::ATan2(sx,cx);

		cz = matrix[0][0] / cy;
		sz = matrix[1][0] / cy;
		euler.z = Math::ATan2(sz,cz);
	}
	else
	{
		matrix[1][1] = 1.0 - (2.0 * x * x) - (2.0 * z * z);
		matrix[1][2] = (2.0 * y * z) - (2.0 * w * x);
		cx = matrix[1][1];
		sx = -matrix[1][2];
		euler.x = Math::ATan2(sx,cx);

		cz = 1.0;
		sz = 0.0;
		euler.z = Math::ATan2(sz,cz);
	}

	return euler;
*/

/*	
	// METHOD #2:
	TMatrix<T> mat = ToMatrix();

	//
	float cy = Math::Sqrt(mat.m44[0][0]*mat.m44[0][0] + mat.m44[0][1]*mat.m44[0][1]);
	if (cy > 16.0*Math::EPSILON)
	{
		result.x = -atan2(mat.m44[1][2], mat.m44[2][2]);
		result.y = -atan2(-mat.m44[0][2], cy);
		result.z = -atan2(mat.m44[0][1], mat.m44[0][0]);
	}
	else
	{
		result.x = -atan2(-mat.m44[2][1], mat.m44[1][1]);
		result.y = -atan2(-mat.m44[0][2], cy);
		result.z = 0.0;
	}

	return result;
*/

	// METHOD #3 (without conversion to matrix first):
	// TODO: safety checks?
	TVector3<T> result;
	T m00 = 1.0 - (2.0 * ((y*y) + z*z));
	T m01 = 2.0 * (x * y + w * z);

	result.x = Math::ATan2( 2.0 * (y * z + w * x), 1.0 - (2.0 * ((x*x) + (y*y))));
	result.y = Math::ATan2(-2.0 * (x * z - w * y), Math::Sqrt((m00*m00) + (m01*m01)));
	result.z = Math::ATan2(m01, m00);

	return result;	
}
