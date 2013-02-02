/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Matrix4.h"


template <class T>
TMatrix<T>::TMatrix(const TMatrix<T>& m)
{
	memcpy(m16, m.m16, sizeof(TMatrix<T>));
}


template <class T>
TMatrix<T>::TMatrix(const TVector3<T>& position, const TQuaternion<T>& rotation, const TVector3<T>& scale)
{
	// convert the quaternion to the matrix
	T x = rotation.x;
	T y = rotation.y;
	T z = rotation.z;
	T w = rotation.w;
    T xx=x*x;
    T xy=x*y, yy=y*y;
    T xz=x*z, yz=y*z, zz=z*z;
    T xw=x*w, yw=y*w, zw=z*w, ww=w*w;
	m44[0][0] = (+xx-yy-zz+ww)*scale.x;	m44[0][1] = (+xy+zw+xy+zw)*scale.y;	m44[0][2] = (+xz-yw+xz-yw)*scale.z;	m44[0][3] = 0;
	m44[1][0] = (+xy-zw+xy-zw)*scale.x;	m44[1][1] = (-xx+yy-zz+ww)*scale.y;	m44[1][2] = (+yz+xw+yz+xw)*scale.z;	m44[1][3] = 0;
	m44[2][0] = (+xz+yw+xz+yw)*scale.x;	m44[2][1] = (+yz-xw+yz-xw)*scale.y;	m44[2][2] = (-xx-yy+zz+ww)*scale.z;	m44[2][3] = 0;
	m44[3][0] = position.x;				m44[3][1] = position.y;				m44[3][2] = position.z;				m44[3][3] = 1;
}


template <class T>
TMatrix<T>::TMatrix(const TVector3<T>& position, const TQuaternion<T>& rotation)
{
	// convert the quaternion to the matrix
	T x = rotation.x;
	T y = rotation.y;
	T z = rotation.z;
	T w = rotation.w;
    T xx=x*x;
    T xy=x*y, yy=y*y;
    T xz=x*z, yz=y*z, zz=z*z;
    T xw=x*w, yw=y*w, zw=z*w, ww=w*w;
	m44[0][0] = +xx-yy-zz+ww;	m44[0][1] = +xy+zw+xy+zw;	m44[0][2] = +xz-yw+xz-yw;	m44[0][3] = 0;
	m44[1][0] = +xy-zw+xy-zw;	m44[1][1] = -xx+yy-zz+ww;	m44[1][2] = +yz+xw+yz+xw;	m44[1][3] = 0;
	m44[2][0] = +xz+yw+xz+yw;	m44[2][1] = +yz-xw+yz-xw;	m44[2][2] = -xx-yy+zz+ww;	m44[2][3] = 0;
	m44[3][0] = position.x;		m44[3][1] = position.y;		m44[3][2] = position.z;		m44[3][3] = 1;
}



template <class T>
TMatrix<T> TMatrix<T>::operator + (const TMatrix<T>& right) const
{
	TMatrix<T> r;
	//const mat44& m = right.m44;

	for (int i=0; i<4; i++)
	{
		r.m44[i][0] = m44[i][0] + right.m44[i][0];
		r.m44[i][1] = m44[i][1] + right.m44[i][1];
		r.m44[i][2] = m44[i][2] + right.m44[i][2];
		r.m44[i][3] = m44[i][3] + right.m44[i][3];
	}

	return r;
}


template <class T>
TMatrix<T> TMatrix<T>::operator - (const TMatrix<T>& right) const
{
	TMatrix<T> r;

	for (int i=0; i<4; i++)
	{
		r.m44[i][0] = m44[i][0] - right.m44[i][0];
		r.m44[i][1] = m44[i][1] - right.m44[i][1];
		r.m44[i][2] = m44[i][2] - right.m44[i][2];
		r.m44[i][3] = m44[i][3] - right.m44[i][3];
	}

	return r;
}



template <class T>
TMatrix<T> TMatrix<T>::operator * (const TMatrix<T>& right) const
{
	TMatrix<T> r;
	//const mat44& m = right.m44;

	unsigned int i;
	for (i=0; i<4; i++)
	{
		const T* v = m16 + i*4;
		r.m44[i][0] = v[0]*right.m44[0][0] + v[1]*right.m44[1][0] + v[2]*right.m44[2][0] + v[3]*right.m44[3][0];
		r.m44[i][1] = v[0]*right.m44[0][1] + v[1]*right.m44[1][1] + v[2]*right.m44[2][1] + v[3]*right.m44[3][1];
		r.m44[i][2] = v[0]*right.m44[0][2] + v[1]*right.m44[1][2] + v[2]*right.m44[2][2] + v[3]*right.m44[3][2];
		r.m44[i][3] = v[0]*right.m44[0][3] + v[1]*right.m44[1][3] + v[2]*right.m44[2][3] + v[3]*right.m44[3][3];
	}

	return r;
}


template <class T>
TMatrix<T>& TMatrix<T>::operator += (const TMatrix<T>& right)
{
	for ( int i=0; i<4; i++ )
	{
		m44[i][0] += right.m44[i][0];
		m44[i][1] += right.m44[i][1];
		m44[i][2] += right.m44[i][2];
		m44[i][3] += right.m44[i][3];
	}
	return *this;
}


template <class T>
TMatrix<T>& TMatrix<T>::operator -= (const TMatrix<T>& right)
{
	//const mat44& m = right.m44;
	for (int i=0; i<4; i++)
	{
		m44[i][0] -= right.m44[i][0];
		m44[i][1] -= right.m44[i][1];
		m44[i][2] -= right.m44[i][2];
		m44[i][3] -= right.m44[i][3];
	}
	return *this;
}


template <class T>
TMatrix<T>& TMatrix<T>::operator *= (const TMatrix<T>& right)
{
	T v[4];
	for (int i=0; i<4; i++)
	{
		v[0] = m44[i][0];
		v[1] = m44[i][1];
		v[2] = m44[i][2];
		v[3] = m44[i][3];
		m44[i][0] = v[0]*right.m44[0][0] + v[1]*right.m44[1][0] + v[2]*right.m44[2][0] + v[3]*right.m44[3][0];
		m44[i][1] = v[0]*right.m44[0][1] + v[1]*right.m44[1][1] + v[2]*right.m44[2][1] + v[3]*right.m44[3][1];
		m44[i][2] = v[0]*right.m44[0][2] + v[1]*right.m44[1][2] + v[2]*right.m44[2][2] + v[3]*right.m44[3][2];
		m44[i][3] = v[0]*right.m44[0][3] + v[1]*right.m44[1][3] + v[2]*right.m44[2][3] + v[3]*right.m44[3][3];
	}
	return *this;
}



template <class T>
inline void TMatrix<T>::operator = (const TMatrix<T>& right)
{
	memcpy(m16, right.m16, sizeof(TMatrix<T>));
}

template <class T>
T TMatrix<T>::CalcDeterminant() const
{
	return
		m44[0][0] * m44[1][1] * m44[2][2] +
		m44[0][1] * m44[1][2] * m44[2][0] +
		m44[0][2] * m44[1][0] * m44[2][1] -
		m44[0][2] * m44[1][1] * m44[2][0] -
		m44[0][1] * m44[1][0] * m44[2][2] -
		m44[0][0] * m44[1][2] * m44[2][1];
}


template <class T>
TVector3<T> TMatrix<T>::CalcEulerAngles() const
{
	#define EulSafe	     "\000\001\002\000"
	#define EulNext	     "\001\002\000\001"
	#define EulGetOrd(ord,i,j,k,n,s,f) {unsigned o=ord;f=o&1;s=o&1; n=o&1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];}

	#define EulOrd(i,p,r,f)	   (((((((i)<<1)+(p))<<1)+(r))<<1)+(f))

	#define EulOrdXYZs    EulOrd(0,0,0,0)
	#define EulOrdXYXs    EulOrd(0,0,1,0)
	#define EulOrdXZYs    EulOrd(0,1,0,0)
	#define EulOrdXZXs    EulOrd(0,1,1,0)
	#define EulOrdYZXs    EulOrd(1,0,0,0)
	#define EulOrdYZYs    EulOrd(1,0,1,0)
	#define EulOrdYXZs    EulOrd(1,1,0,0)
	#define EulOrdYXYs    EulOrd(1,1,1,0)
	#define EulOrdZXYs    EulOrd(2,0,0,0)
	#define EulOrdZXZs    EulOrd(2,0,1,0)
	#define EulOrdZYXs    EulOrd(2,1,0,0)
	#define EulOrdZYZs    EulOrd(2,1,1,0)

	#define EulOrdZYXr    EulOrd(0,0,0,1)
	#define EulOrdXYXr    EulOrd(0,0,1,1)
	#define EulOrdYZXr    EulOrd(0,1,0,1)
	#define EulOrdXZXr    EulOrd(0,1,1,1)
	#define EulOrdXZYr    EulOrd(1,0,0,1)
	#define EulOrdYZYr    EulOrd(1,0,1,1)
	#define EulOrdZXYr    EulOrd(1,1,0,1)
	#define EulOrdYXYr    EulOrd(1,1,1,1)
	#define EulOrdYXZr    EulOrd(2,0,0,1)
	#define EulOrdZXZr    EulOrd(2,0,1,1)
	#define EulOrdXYZr    EulOrd(2,1,0,1)
	#define EulOrdZYZr    EulOrd(2,1,1,1)

	int order = EulOrdXYZs;
	int i,j,k,n,s,f;

	EulGetOrd(order,i,j,k,n,s,f);
	TVector3<T> v;

	if ( s == 1 )
	{
		T sy = Math::Sqrt( m44[j][i]*m44[j][i] + m44[k][i]*m44[k][i] );
		if ( sy > Math::EPSILON*16 )
		{
			v.x = Math::ATan2( m44[j][i],m44[k][i] );
			v.y = Math::ATan2( sy,m44[i][i] );
			v.z = Math::ATan2( m44[i][j],-m44[i][k] );
		}
		else
		{
			v.x = Math::ATan2( -m44[k][j],m44[j][j] );
			v.y = Math::ATan2( sy,m44[i][i] );
			v.z = 0;
		}
	}
	else
	{
		T cy = Math::Sqrt( m44[i][i]*m44[i][i] + m44[i][j]*m44[i][j] );
		if ( cy > Math::EPSILON*16 )
		{
			v.x = Math::ATan2( m44[j][k],m44[k][k] );
			v.y = Math::ATan2( -m44[i][k],cy );
			v.z = Math::ATan2( m44[i][j],m44[i][i] );
		} 
		else 
		{
			v.x = Math::ATan2( -m44[k][j],m44[j][j] );
			v.y = Math::ATan2( -m44[i][k],cy);
			v.z = 0;
		}
	}
	
    if ( n==1 ) { v = -v; }
    if ( f==1 ) { T t = v.x; v.x = v.z;	v.z = t; }

	return v;
}


template <class T>
void TMatrix<T>::Identity()
{
	m44[0][0] = 1; m44[0][1] = 0; m44[0][2] = 0; m44[0][3] = 0;
	m44[1][0] = 0; m44[1][1] = 1; m44[1][2] = 0; m44[1][3] = 0;
	m44[2][0] = 0; m44[2][1] = 0; m44[2][2] = 1; m44[2][3] = 0;
	m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
}


template <class T>
void TMatrix<T>::SetScaleMatrix(const T x, const T y, const T z)
{
	m44[0][0] = x; m44[0][1] = 0; m44[0][2] = 0; m44[0][3] = 0;
	m44[1][0] = 0; m44[1][1] = y; m44[1][2] = 0; m44[1][3] = 0;
	m44[2][0] = 0; m44[2][1] = 0; m44[2][2] = z; m44[2][3] = 0;
	m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
}


template <class T>
void TMatrix<T>::SetTranslationMatrix(const T x, const T y, const T z)
{
	m44[0][0] = 1; m44[0][1] = 0; m44[0][2] = 0; m44[0][3] = 0;
	m44[1][0] = 0; m44[1][1] = 1; m44[1][2] = 0; m44[1][3] = 0;
	m44[2][0] = 0; m44[2][1] = 0; m44[2][2] = 1; m44[2][3] = 0;
	m44[3][0] = x; m44[3][1] = y; m44[3][2] = z; m44[3][3] = 1;
}

template <class T>
void TMatrix<T>::SetRotationMatrixX(const T angle)
{
	const T s = Math::Sin( angle );
	const T c = Math::Cos( angle );
	
	m44[0][0] = 1; m44[0][1] = 0; m44[0][2] = 0; m44[0][3] = 0;
	m44[1][0] = 0; m44[1][1] = c; m44[1][2] = s; m44[1][3] = 0;
	m44[2][0] = 0; m44[2][1] =-s; m44[2][2] = c; m44[2][3] = 0;
	m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
}


template <class T>
void TMatrix<T>::SetRotationMatrixY(const T angle)
{
	const T s = Math::Sin( angle );
	const T c = Math::Cos( angle );
	
	m44[0][0] = c; m44[0][1] = 0; m44[0][2] =-s; m44[0][3] = 0;
	m44[1][0] = 0; m44[1][1] = 1; m44[1][2] = 0; m44[1][3] = 0;
	m44[2][0] = s; m44[2][1] = 0; m44[2][2] = c; m44[2][3] = 0;
	m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
}


template <class T>
void TMatrix<T>::SetRotationMatrixZ(const T angle)
{
	const T s = Math::Sin( angle );
	const T c = Math::Cos( angle );

	m44[0][0] = c; m44[0][1] = s; m44[0][2] = 0; m44[0][3] = 0;
	m44[1][0] =-s; m44[1][1] = c; m44[1][2] = 0; m44[1][3] = 0;
	m44[2][0] = 0; m44[2][1] = 0; m44[2][2] = 1; m44[2][3] = 0;
	m44[3][0] = 0; m44[3][1] = 0; m44[3][2] = 0; m44[3][3] = 1;
}

/*
template <class T>
void TMatrix<T>::SetRotationMatrixXYZ(const T yaw, const T pitch, const T roll)
{
	const T sy = Math::Sin(yaw);
	const T cy = Math::Cos(yaw);
	const T sp = Math::Sin(pitch);
	const T cp = Math::Cos(pitch);
	const T sr = Math::Sin(roll);
	const T cr = Math::Cos(roll);
	const T spsy = sp * sy;
	const T spcy = sp * cy;

	m44[0][0] = cr * cp;
	m44[0][1] = sr * cp;
	m44[0][2] = -sp;
	m44[0][3] = 0;
	m44[1][0] = cr * spsy - sr * cy;
	m44[1][1] = sr * spsy + cr * cy;
	m44[1][2] = cp * sy;
	m44[1][3] = 0;
	m44[2][0] = cr * spcy + sr * sy;
	m44[2][1] = sr * spcy - cr * sy;
	m44[2][2] = cp * cy;
	m44[2][3] = 0;
	m44[3][0] = 0;
	m44[3][1] = 0;
	m44[3][2] = 0;
	m44[3][3] = 1;
}
*/

template <class T>
void TMatrix<T>::SetRotationMatrixEuler(const TVector3<T>& v)
{
	*this = TMatrix<T>::RotationMatrixX(v.x) * TMatrix<T>::RotationMatrixY(v.y) * TMatrix<T>::RotationMatrixZ(v.z);
}


template <class T>
void TMatrix<T>::SetRotationMatrixAxisAngle(const TVector3<T>& axis, const T angle)
{
	T length2 = axis.SquareLength();
	if ( !length2 )
	{
		Identity();
		return;
	}
	
	TVector3<T> n = axis / Math::Sqrt(length2);
	const T s = Math::Sin(angle);
	const T c = Math::Cos(angle);
	const T k = 1 - c;
	const T xx = n.x * n.x * k + c;
	const T yy = n.y * n.y * k + c;
	const T zz = n.z * n.z * k + c;
	const T xy = n.x * n.y * k;
	const T yz = n.y * n.z * k;
	const T zx = n.z * n.x * k;
	const T xs = n.x * s;
	const T ys = n.y * s;
	const T zs = n.z * s;
	
	m44[0][0] = xx;
	m44[0][1] = xy + zs;
	m44[0][2] = zx - ys;
	m44[0][3] = 0;
	m44[1][0] = xy - zs;
	m44[1][1] = yy;
	m44[1][2] = yz + xs;
	m44[1][3] = 0;
	m44[2][0] = zx + ys;
	m44[2][1] = yz - xs;
	m44[2][2] = zz;
	m44[2][3] = 0;
	m44[3][0] = 0;
	m44[3][1] = 0;
	m44[3][2] = 0;
	m44[3][3] = 1;
}


template <class T>
void TMatrix<T>::Scale(const T sx, const T sy, const T sz)
{
	for (int i=0; i<4; i++)
	{
		m44[i][0] *= sx;
		m44[i][1] *= sy;
		m44[i][2] *= sz;
	}
}


template <class T>
void TMatrix<T>::RotateX(const T angle)
{
	T s = Math::Sin( angle );
	T c = Math::Cos( angle );
	
	for (int i=0; i<4; i++)
	{
		const T y = m44[i][1];
		const T z = m44[i][2];
		m44[i][1] = y * c - z * s;
		m44[i][2] = z * c + y * s;
	}
}


template <class T>
void TMatrix<T>::RotateY(const T angle)
{
	T s = Math::Sin( angle );
	T c = Math::Cos( angle );
	
	for (int i=0; i<4; i++)
	{
		const T x = m44[i][0];
		const T z = m44[i][2];
		m44[i][0] = x * c + z * s;
		m44[i][2] = z * c - x * s;
	}
}


template <class T>
void TMatrix<T>::RotateZ(const T angle)
{
	T s = Math::Sin( angle );
	T c = Math::Cos( angle );
	
	for (int i=0; i<4; i++)
	{
		const T x = m44[i][0];
		const T y = m44[i][1];
		m44[i][0] = x * c - y * s;
		m44[i][1] = x * s + y * c;
	}
}

/*
template <class T>
void TMatrix<T>::RotateXYZ(const T yaw, const T pitch, const T roll)
{
	const T sy = Math::Sin(yaw);
	const T cy = Math::Cos(yaw);
	const T sp = Math::Sin(pitch);
	const T cp = Math::Cos(pitch);
	const T sr = Math::Sin(roll);
	const T cr = Math::Cos(roll);
	const T spsy = sp * sy;
	const T spcy = sp * cy;
	const T m00 = cr * cp;
	const T m01 = sr * cp;
	const T m02 = -sp;
	const T m10 = cr * spsy - sr * cy;
	const T m11 = sr * spsy + cr * cy;
	const T m12 = cp * sy;
	const T m20 = cr * spcy + sr * sy;
	const T m21 = sr * spcy - cr * sy;
	const T m22 = cp * cy;

	for ( int i=0; i<4; i++ )
	{
		const T x = m44[i][0];
		const T y = m44[i][1];
		const T z = m44[i][2];
		m44[i][0] = x * m00 + y * m10 + z * m20;
		m44[i][1] = x * m01 + y * m11 + z * m21;
		m44[i][2] = x * m02 + y * m12 + z * m22;
	}
}
*/

template <class T>
void TMatrix<T>::MultMatrix(const TMatrix<T>& right)
{	
	T v[4];

	for (int i=0; i<4; i++)
	{
		v[0] = m44[i][0];
		v[1] = m44[i][1];
		v[2] = m44[i][2];
		v[3] = m44[i][3];
		m44[i][0] = v[0]*right.m44[0][0] + v[1]*right.m44[1][0] + v[2]*right.m44[2][0] + v[3]*right.m44[3][0];
		m44[i][1] = v[0]*right.m44[0][1] + v[1]*right.m44[1][1] + v[2]*right.m44[2][1] + v[3]*right.m44[3][1];
		m44[i][2] = v[0]*right.m44[0][2] + v[1]*right.m44[1][2] + v[2]*right.m44[2][2] + v[3]*right.m44[3][2];
		m44[i][3] = v[0]*right.m44[0][3] + v[1]*right.m44[1][3] + v[2]*right.m44[2][3] + v[3]*right.m44[3][3];
	}
}


template <class T>
void TMatrix<T>::MultMatrix3x4(const TMatrix<T>& right)
{
	//const mat44& m = right.m44;
	T v[3];

	for (int i=0; i<4; i++)
	{
		v[0] = m44[i][0];
		v[1] = m44[i][1];
		v[2] = m44[i][2];
		m44[i][0] = v[0]*right.m44[0][0] + v[1]*right.m44[1][0] + v[2]*right.m44[2][0];
		m44[i][1] = v[0]*right.m44[0][1] + v[1]*right.m44[1][1] + v[2]*right.m44[2][1];
		m44[i][2] = v[0]*right.m44[0][2] + v[1]*right.m44[1][2] + v[2]*right.m44[2][2];
	}

	m44[3][0] += right.m44[3][0];
	m44[3][1] += right.m44[3][1];
	m44[3][2] += right.m44[3][2];
}


template <class T>
void TMatrix<T>::MultMatrix3x3(const TMatrix<T>& right)
{
	T v[3];

	for ( int i=0; i<4; i++ )
	{
		v[0] = m44[i][0];
		v[1] = m44[i][1];
		v[2] = m44[i][2];
		m44[i][0] = v[0]*right.m44[0][0] + v[1]*right.m44[1][0] + v[2]*right.m44[2][0];
		m44[i][1] = v[0]*right.m44[0][1] + v[1]*right.m44[1][1] + v[2]*right.m44[2][1];
		m44[i][2] = v[0]*right.m44[0][2] + v[1]*right.m44[1][2] + v[2]*right.m44[2][2];
	}
}


template <class T>
void TMatrix<T>::MultInverseMatrix3x3(const TMatrix<T>& right)
{
	const T m00 = m44[0][0];
	const T m01 = m44[0][1];
	const T m02 = m44[0][2];
	const T m10 = m44[1][0];
	const T m11 = m44[1][1];
	const T m12 = m44[1][2];
	const T m20 = m44[2][0];
	const T m21 = m44[2][1];
	const T m22 = m44[2][2];

	for (int i=0; i<4; i++)
	{
		const T* v = right.m16 + i*4;
		m44[i][0] = v[0]*m00 + v[1]*m10 + v[2]*m20;
		m44[i][1] = v[0]*m01 + v[1]*m11 + v[2]*m21;
		m44[i][2] = v[0]*m02 + v[1]*m12 + v[2]*m22;
	}
}


template <class T>
void TMatrix<T>::Transpose()
{
	TMatrix<T> v;

	v.m44[0][0] = m44[0][0];
	v.m44[0][1] = m44[1][0];
	v.m44[0][2] = m44[2][0];
	v.m44[0][3] = m44[3][0];
	v.m44[1][0] = m44[0][1];
	v.m44[1][1] = m44[1][1];
	v.m44[1][2] = m44[2][1];
	v.m44[1][3] = m44[3][1];
	v.m44[2][0] = m44[0][2];
	v.m44[2][1] = m44[1][2];
	v.m44[2][2] = m44[2][2];
	v.m44[2][3] = m44[3][2];
	v.m44[3][0] = m44[0][3];
	v.m44[3][1] = m44[1][3];
	v.m44[3][2] = m44[2][3];
	v.m44[3][3] = m44[3][3];

	*this = v;
}


template <class T>
void TMatrix<T>::TransposeRot()
{
	TMatrix<T> v(*this);

	v.m44[0][0] = m44[0][0];
	v.m44[0][1] = m44[1][0];
	v.m44[0][2] = m44[2][0];
	v.m44[1][0] = m44[0][1];
	v.m44[1][1] = m44[1][1];
	v.m44[1][2] = m44[2][1];
	v.m44[2][0] = m44[0][2];
	v.m44[2][1] = m44[1][2];
	v.m44[2][2] = m44[2][2];

	*this = v;
}


template <class T>
void TMatrix<T>::TransposeTranslation()
{
	TVector3<T> temp;

	temp.x = m44[3][0];
	temp.y = m44[3][1];
	temp.z = m44[3][2];

	m44[3][0] = m44[0][3];
	m44[3][1] = m44[1][3];
	m44[3][2] = m44[2][3];

	m44[0][3] = temp.x;
	m44[1][3] = temp.y;
	m44[2][3] = temp.z;
}


template <class T>
void TMatrix<T>::Adjoint()
{
	TMatrix<T> v;

	v.m44[0][0] = m44[1][1]*m44[2][2] - m44[1][2]*m44[2][1];
	v.m44[0][1] = m44[2][1]*m44[0][2] - m44[2][2]*m44[0][1];
	v.m44[0][2] = m44[0][1]*m44[1][2] - m44[0][2]*m44[1][1];
	v.m44[0][3] = m44[0][3];
	v.m44[1][0] = m44[1][2]*m44[2][0] - m44[1][0]*m44[2][2];
	v.m44[1][1] = m44[2][2]*m44[0][0] - m44[2][0]*m44[0][2];
	v.m44[1][2] = m44[0][2]*m44[1][0] - m44[0][0]*m44[1][2];
	v.m44[1][3] = m44[1][3];
	v.m44[2][0] = m44[1][0]*m44[2][1] - m44[1][1]*m44[2][0];
	v.m44[2][1] = m44[2][0]*m44[0][1] - m44[2][1]*m44[0][0];
	v.m44[2][2] = m44[0][0]*m44[1][1] - m44[0][1]*m44[1][0];
	v.m44[2][3] = m44[2][3];
	v.m44[3][0] = -(m44[0][0]*m44[3][0] + m44[1][0]*m44[3][1] + m44[2][0]*m44[3][2]);
	v.m44[3][1] = -(m44[0][1]*m44[3][0] + m44[1][1]*m44[3][1] + m44[2][1]*m44[3][2]);
	v.m44[3][2] = -(m44[0][2]*m44[3][0] + m44[1][2]*m44[3][1] + m44[2][2]*m44[3][2]);
	v.m44[3][3] = m44[3][3];

	*this = v;
}


template <class T>
TVector3<T>	TMatrix<T>::InverseRot(const TVector3<T> &v)
{
	TMatrix<T> m(*this);
	m.Inverse();
	m.SetTranslation(0, 0, 0);
	return v * m;
}


template <class T>
void TMatrix<T>::Inverse()
{
	T s = 1.0 / CalcDeterminant();
	TMatrix<T> v;

	v.m44[0][0] = (m44[1][1]*m44[2][2] - m44[1][2]*m44[2][1]) * s;
	v.m44[0][1] = (m44[2][1]*m44[0][2] - m44[2][2]*m44[0][1]) * s;
	v.m44[0][2] = (m44[0][1]*m44[1][2] - m44[0][2]*m44[1][1]) * s;
	v.m44[0][3] = m44[0][3];
	v.m44[1][0] = (m44[1][2]*m44[2][0] - m44[1][0]*m44[2][2]) * s;
	v.m44[1][1] = (m44[2][2]*m44[0][0] - m44[2][0]*m44[0][2]) * s;
	v.m44[1][2] = (m44[0][2]*m44[1][0] - m44[0][0]*m44[1][2]) * s;
	v.m44[1][3] = m44[1][3];
	v.m44[2][0] = (m44[1][0]*m44[2][1] - m44[1][1]*m44[2][0]) * s;
	v.m44[2][1] = (m44[2][0]*m44[0][1] - m44[2][1]*m44[0][0]) * s;
	v.m44[2][2] = (m44[0][0]*m44[1][1] - m44[0][1]*m44[1][0]) * s;
	v.m44[2][3] = m44[2][3];
	v.m44[3][0] =-(v.m44[0][0]*m44[3][0] + v.m44[1][0]*m44[3][1] + v.m44[2][0]*m44[3][2]);
	v.m44[3][1] =-(v.m44[0][1]*m44[3][0] + v.m44[1][1]*m44[3][1] + v.m44[2][1]*m44[3][2]);
	v.m44[3][2] =-(v.m44[0][2]*m44[3][0] + v.m44[1][2]*m44[3][1] + v.m44[2][2]*m44[3][2]);
	v.m44[3][3] = m44[3][3];

	*this = v;
}


template <class T>
void TMatrix<T>::OrthoNormalize()
{
	TVector3<T> x = GetRight();
	TVector3<T> y = GetUp();
	TVector3<T> z = GetForward();

	x.Normalize();
	y -= x * x.Dot(y);
	y.Normalize();
	z = x.Cross(y);

	SetRight( x );
	SetUp( y );
	SetForward( z );
}


template <class T>
void TMatrix<T>::Mirror(const TMatrix<T>& transform, const TPlaneEq<T>& plane)
{
	// components
	TVector3<T> x = transform.GetRight();
	TVector3<T> y = transform.GetUp();
	TVector3<T> z = transform.GetForward();
	TVector3<T> t = transform.GetTranslation();
	TVector3<T> n = plane.GetNormal();
	TVector3<T> n2= n * -2;
	T			d = plane.GetDist();

	// mirror translation
	TVector3<T> mt = t + n2 * (t.Dot(n) - d);
	
	// mirror x rotation
	x += t;
	x += n2 * (x.Dot(n) - d);
	x -= mt;

	// mirror y rotation
	y += t;
	y += n2 * (y.Dot(n) - d);
	y -= mt;

	// mirror z rotation
	z += t;
	z += n2 * (z.Dot(n) - d);
	z -= mt;

	// write result
	SetRight( x );
	SetUp( y );
	SetForward( z );
	SetTranslation( mt );

	m44[0][3] = 0; 
	m44[1][3] = 0; 
	m44[2][3] = 0; 
	m44[3][3] = 1;
}


template <class T>
void TMatrix<T>::LookAt(const TVector3<T>& view, const TVector3<T>& target, const TVector3<T>& up)
{
	TVector3<T> z = (target-view).Normalize();
	TVector3<T> x = (up.Cross(z)).Normalize();
	TVector3<T> y = z.Cross(x);

    m44[0][0] = x.x;
    m44[1][0] = x.y;
    m44[2][0] = x.z;
    m44[3][0] = -x.Dot(view);
    m44[0][1] = y.x;
    m44[1][1] = y.y;
    m44[2][1] = y.z;
    m44[3][1] = -y.Dot(view);
    m44[0][2] = z.x;
    m44[1][2] = z.y;
    m44[2][2] = z.z;
    m44[3][2] = -z.Dot(view);
    m44[0][3] = 0;
    m44[1][3] = 0;
    m44[2][3] = 0;
    m44[3][3] = 1;
}



// ortho projection TMatrix<T>
template <class T>
void TMatrix<T>::Ortho(const T left, const T right, const T top, const T bottom, const T znear, const T zfar)
{
	m44[0][0] = 2.0 / (right-left);
	m44[1][0] = 0.0;
	m44[2][0] = 0.0;
	m44[3][0] = (right + left) / (right - left);
	m44[0][1] = 0.0;
	m44[1][1] = 2.0 / (top - bottom);
	m44[2][1] = 0.0;
	m44[3][0] = (top + bottom) / (top - bottom);
	m44[0][2] = 0.0;
	m44[1][2] = 0.0;
	m44[2][2] = 2.0 / (zfar - znear);
	m44[3][2] = (zfar + znear) / (znear - zfar);
	m44[0][3] = 0.0;
	m44[1][3] = 0.0;
	m44[2][3] = 0.0;
	m44[3][3] = 1.0;
}


// frustum TMatrix<T>
template <class T>
void TMatrix<T>::Frustum(const T left, const T right, const T top, const T bottom, const T znear, const T zfar)
{
	m44[0][0] = 2.0*znear / (right - left);
	m44[1][0] = 0.0;
	m44[2][0] = (right + left) / (right - left);
	m44[3][0] = 0.0;
	m44[0][1] = 0.0;
	m44[1][1] = 2.0*znear / (top - bottom);  
	m44[2][1] = (top + bottom) / (top - bottom);
	m44[3][1] = 0.0;
	m44[0][2] = 0.0;
	m44[1][2] = 0.0;
	m44[2][2] = (zfar + znear) / (zfar - znear);
	m44[3][2] = 2.0*zfar*znear / (zfar - znear);
	m44[0][3] = 0.0;
	m44[1][3] = 0.0;
	m44[2][3] = -1.0;
	m44[3][3] = 0.0;
}


// setup projection TMatrix<T>
template <class T>
void TMatrix<T>::Projection(const T fov, const T aspect, const T zNear, const T zFar)
{
	const T f = fov * 0.5;
	const T w = aspect * (Math::Cos(f) / Math::Sin(f));
	const T h = Math::Cos(f) / Math::Sin(f);
	const T Q = zFar / (zFar - zNear);

	memset(m44, 0, 16*sizeof(T));
	m44[0][0] = w;
	m44[1][1] = h;
	m44[2][2] = Q;
	m44[2][3] = 1.0;
	m44[3][2] = -Q * zNear;
}


template <class T>
inline void TMatrix<T>::SetRight(const T xx, const T xy, const T xz)
{
	m44[0][0] = xx;
	m44[0][1] = xy;
	m44[0][2] = xz;
}


template <class T>
inline void TMatrix<T>::SetUp(const T yx, const T yy, const T yz)
{
	m44[1][0] = yx;
	m44[1][1] = yy;
	m44[1][2] = yz;
}


template <class T>
inline void TMatrix<T>::SetForward(const T zx, const T zy, const T zz)
{
	m44[2][0] = zx;
	m44[2][1] = zy;
	m44[2][2] = zz;
}


template <class T>
inline void TMatrix<T>::SetTranslation(const T tx, const T ty, const T tz)
{
	m44[3][0] = tx;
	m44[3][1] = ty;
	m44[3][2] = tz;
}


template <class T>
inline void TMatrix<T>::SetRight(const TVector3<T>& x)
{
	m44[0][0] = x.x;
	m44[0][1] = x.y;
	m44[0][2] = x.z;
}


template <class T>
inline void TMatrix<T>::SetUp(const TVector3<T>& y)
{
	m44[1][0] = y.x;
	m44[1][1] = y.y;
	m44[1][2] = y.z;
}


template <class T>
inline void TMatrix<T>::SetForward(const TVector3<T>& z)
{
	m44[2][0] = z.x;
	m44[2][1] = z.y;
	m44[2][2] = z.z;
}


template <class T>
inline void TMatrix<T>::SetTranslation(const TVector3<T>& t)
{
	m44[3][0] = t.x;
	m44[3][1] = t.y;
	m44[3][2] = t.z;
}


template <class T>
inline TVector3<T>& TMatrix<T>::GetRight()
{
	return *reinterpret_cast<TVector3<T>*>(m16+0);
}


template <class T>
inline TVector3<T>& TMatrix<T>::GetUp()
{
	return *reinterpret_cast<TVector3<T>*>(m16+4);
}


template <class T>
inline TVector3<T>& TMatrix<T>::GetForward()
{
	return *reinterpret_cast<TVector3<T>*>(m16+8);
}


template <class T>
inline TVector3<T>& TMatrix<T>::GetTranslation()
{
	return *reinterpret_cast<TVector3<T>*>(m16+12);
}


template <class T>
inline const TVector3<T>& TMatrix<T>::GetRight() const
{
	return *reinterpret_cast<const TVector3<T>*>(m16+0);
}


template <class T>
inline const TVector3<T>& TMatrix<T>::GetUp() const
{
	return *reinterpret_cast<const TVector3<T>*>(m16+4);
}


template <class T>
inline const TVector3<T>& TMatrix<T>::GetForward() const
{
	return *reinterpret_cast<const TVector3<T>*>(m16+8);
}


template <class T>
inline const TVector3<T>& TMatrix<T>::GetTranslation() const
{
	return *reinterpret_cast<const TVector3<T>*>(m16+12);
}


template <class T>
inline	TVector3<T>	TMatrix<T>::Mul3x3(const TVector3<T> &v)
{
	return TVector3<T>(
		v.x*m44[0][0] + v.y*m44[1][0] + v.z*m44[2][0],
		v.x*m44[0][1] + v.y*m44[1][1] + v.z*m44[2][1],
		v.x*m44[0][2] + v.y*m44[1][2] + v.z*m44[2][2] );
}


template <class T>
inline void operator *= (TVector3<T>& v, const TMatrix<T>& m)
{
	v =	TVector3<T>(
		v.x*m.m44[0][0] + v.y*m.m44[1][0] + v.z*m.m44[2][0] + m.m44[3][0],
		v.x*m.m44[0][1] + v.y*m.m44[1][1] + v.z*m.m44[2][1] + m.m44[3][1],
		v.x*m.m44[0][2] + v.y*m.m44[1][2] + v.z*m.m44[2][2] + m.m44[3][2] );
}


template <class T>
inline TVector3<T> operator * (const TVector3<T>& v, const TMatrix<T>& m)
{
	return TVector3<T>(
		v.x*m.m44[0][0] + v.y*m.m44[1][0] + v.z*m.m44[2][0] + m.m44[3][0],
		v.x*m.m44[0][1] + v.y*m.m44[1][1] + v.z*m.m44[2][1] + m.m44[3][1],
		v.x*m.m44[0][2] + v.y*m.m44[1][2] + v.z*m.m44[2][2] + m.m44[3][2] );
}


// check if the matrix is symmetric or not
template <class T>
bool TMatrix<T>::IsSymmetric(const float tollerance) const
{
	// if no tollerance check is needed
	if (tollerance == 0)
	{
		if (m44[1][0] != m44[0][1]) return false;
		if (m44[2][0] != m44[0][2]) return false;
		if (m44[2][1] != m44[1][2]) return false;
		if (m44[3][0] != m44[0][3]) return false;
		if (m44[3][1] != m44[1][3]) return false;
		if (m44[3][2] != m44[2][3]) return false;
	}
	else // tollerance check needed
	{
		if (Math::Abs(m44[1][0] - m44[0][1]) > tollerance) return false;
		if (Math::Abs(m44[2][0] - m44[0][2]) > tollerance) return false;
		if (Math::Abs(m44[2][1] - m44[1][2]) > tollerance) return false;
		if (Math::Abs(m44[3][0] - m44[0][3]) > tollerance) return false;
		if (Math::Abs(m44[3][1] - m44[1][3]) > tollerance) return false;
		if (Math::Abs(m44[3][2] - m44[2][3]) > tollerance) return false;
	}

	// yeah, we have a symmetric matrix here
	return true;
}


// check if this matrix is a diagonal matrix or not.
template <class T>
bool TMatrix<T>::IsDiagonal(const float tollerance) const
{
	if (tollerance == 0)
	{
		// for all entries
		for (int y=0; y<4; y++)
		{
			for (int x=0; x<4; x++)
			{
				// if we are on the diagonal
				if (x == y)
				{
					if (m44[y][x] == 0) return false;	// if this entry on the diagonal is 0, we have no diagonal matrix
				}
				else // we are not on the diagonal
					if (m44[y][x] != 0) return false;	// if the entry isn't equal to 0, it isn't a diagonal matrix
			}
		}
	}
	else
	{
		// for all entries
		for (int y=0; y<4; y++)
		{
			for (int x=0; x<4; x++)
			{
				// if we are on the diagonal
				if (x == y)
				{
					if (Math::Abs(m44[y][x]) < tollerance)
						return false;	// if this entry on the diagonal is 0, we have no diagonal matrix
				}
				else // we are not on the diagonal
				{
					if (Math::Abs(m44[y][x]) > tollerance)
						return false;	// if the entry isn't equal to 0, it isn't a diagonal matrix
				}
			}
		}
	}

	// yeaaah, we have a diagonal matrix here
	return true;
}


// prints the matrix into the logfile or debug output, using MCore::LOG()
template <class T>
void TMatrix<T>::Print()
{
	MCore::LOG("");
	MCore::LOG("[%.8f, %.8f, %.8f, %.8f]", m44[0][0], m44[0][1], m44[0][2], m44[0][3]);
	MCore::LOG("[%.8f, %.8f, %.8f, %.8f]", m44[1][0], m44[1][1], m44[1][2], m44[1][3]);
	MCore::LOG("[%.8f, %.8f, %.8f, %.8f]", m44[2][0], m44[2][1], m44[2][2], m44[2][3]);
	MCore::LOG("[%.8f, %.8f, %.8f, %.8f]", m44[3][0], m44[3][1], m44[3][2], m44[3][3]);
	MCore::LOG("");
}


// check if the matrix is orthogonal or not
template <class T>
bool TMatrix<T>::IsOrthogonal(const float tollerance) const
{
	// get the matrix vectors
	TVector3<T> right	= GetRight();
	TVector3<T> up		= GetUp();
	TVector3<T> forward = GetForward();

	// check if the vectors form an orthonormal set
	if ( Math::Abs( right.Dot( up )      > tollerance) ) return false;
	if ( Math::Abs( right.Dot( forward ) > tollerance) ) return false;
	if ( Math::Abs( forward.Dot( up )    > tollerance) ) return false;

	// the vector set is not orthonormal, so the matrix is not an orthogonal one
	return true;
}


// check if the matrix is an identity matrix or not
template <class T>
bool TMatrix<T>::IsIdentity(const float tollerance) const
{
	// for all entries
	for (int y=0; y<4; y++)
	{
		for (int x=0; x<4; x++)
		{
			// if we are on the diagonal
			if (x == y)
			{
				if (Math::Abs(1.0 - m44[y][x]) > tollerance)
					return false;	// if this entry on the diagonal not 1, we have no identity matrix
			}
			else // we are not on the diagonal
			{
				if (Math::Abs(m44[y][x]) > tollerance)
					return false;	// if the entry isn't equal to 0, it isn't an identity matrix
			}
		}
	}

	// yup, we have an identity matrix here :)
	return true;
}


// calculate the handedness of the matrix
template <class T>
T TMatrix<T>::CalcHandedness() const
{
	// get the matrix vectors
	TVector3<T> right	= GetRight();
	TVector3<T> up		= GetUp();
	TVector3<T> forward = GetForward();

	// calculate the handedness (negative result means left handed, positive means right handed)
	return (right.Cross(up)).Dot( forward );
}


// check if the matrix is right handed or not
template <class T>
inline bool TMatrix<T>::IsRightHanded() const
{
	return (CalcHandedness() > 0);
}


// check if the matrix is right handed or not
template <class T>
inline bool TMatrix<T>::IsLeftHanded() const
{
	return (CalcHandedness() <= 0);
}


// check if this matrix is a pure rotation matrix or not
template <class T>
inline bool TMatrix<T>::IsPureRotationMatrix(const float tollerance) const
{
	return (Math::Abs(1.0 - CalcDeterminant()) < tollerance);
}


// check if the matrix is reflected (mirrored) or not
template <class T>
inline bool TMatrix<T>::IsReflective(const float tollerance) const
{
	T determinant = CalcDeterminant();
	return (determinant < 0);
	//return ((determinant > (-1.0 - tollerance)) && (determinant < (-1.0 + tollerance)));	// if the determinant is near -1, it will reflect
}


// calculate the inverse transpose
template <class T>
inline void TMatrix<T>::InverseTranspose()
{
	Inverse();
	Transpose();
}


// return the inverse transposed version of this matrix
template <class T>
inline TMatrix<T> TMatrix<T>::InverseTransposed() const
{
	Matrix result( *this );
	result.InverseTranspose();
	return result;
}


// normalize a matrix
template <class T>
void TMatrix<T>::Normalize()
{
	// get the current vectors
	TVector3<T> right	= GetRight();
	TVector3<T> up		= GetUp();
	TVector3<T> forward	= GetForward();

	// normalize them
	right.Normalize();
	up.Normalize();
	forward.Normalize();

	// update them again with the normalized versions
	SetRight( right );
	SetUp( up );
	SetForward( forward );
}


// returns a normalized version of this matrix
template <class T>
inline TMatrix<T> TMatrix<T>::Normalized() const
{
	Matrix result( *this );
	result.Normalize();
	return result;
}

