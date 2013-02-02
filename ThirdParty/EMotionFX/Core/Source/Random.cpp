/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Macros.h"
#include "Random.h"
#include <math.h>

namespace MCore
{

// returns a random direction vector
Vector3 Random::RandDirVecF()
{
	float z = RandF(-1.0f, 1.0f);
	float a = RandF(0.0f, Math::TWO_PI);

	float r = sqrtf(1.0f - z*z);

	float x = r * cosf(a);
	float y = r * sinf(a);

	return Vector3(x, y, z);
}


// returns a random direction vector
Vector3 Random::RandDirVecD()
{
	double z = RandF(-1.0, 1.0);
	double a = RandF(0.0, Math::TWO_PI);

	double r = Math::Sqrt(1.0 - z*z);

	double x = r * Math::Cos(a);
	double y = r * Math::Sin(a);

	return Vector3(x, y, z);
}


// returns a random direction vector, using <dir> as basis vector with a cone of <coneAngle> radians
Vector3 Random::RandomDirVec(const Vector3& dir, float coneAngle)
{
	// determine r, phi and theta of direction vector
	float r = dir.Length();
	if (r == 0)	return Vector3(0, 0, 1);

	float oneOverR		= 1/r;
	float theta			= Math::ACos(dir.z * oneOverR);
	float phi			= atan2(dir.y, dir.x);
	float halfConeAngle = 0.5f*coneAngle;					// set up random vector within a cone that points upwards
	float newPhi		= RandF(0, Math::TWO_PI);
	float newTheta		= RandF(0, halfConeAngle);
	float sa			= Math::Sin(newTheta);
	Vector3 result(Math::Cos(newPhi)*sa, Math::Sin(newPhi)*sa, Math::Cos(newTheta));

	// rotate the random vector towards the direction vector
	float ca	= Math::Cos(-theta);
	float sb	= Math::Sin(-phi);
	float cb	= Math::Cos(-phi);
	sa			= Math::Sin(-theta);

	Vector3 oldResult(result);
	result.z	= oldResult.x*sa + oldResult.z*ca;
	oldResult.x = oldResult.x*ca - oldResult.z*sa;
	result.x	= sb*oldResult.y + cb*oldResult.x;
	result.y	= cb*oldResult.y - sb*oldResult.x;

	return result;
} 




// returns a random direction vector, using <dir> as basis vector with a cone of <coneAngle> radians, sampled on a <gridSizeX x gridSizeY> grid
// and it will return a random position inside gridcell [xGridPos][yGridPos]
Vector3 Random::RandomDirVec(const Vector3& dir, float coneAngle, int gridSizeX, int gridSizeY, int xGridPos, int yGridPos)
{
	// determine r, phi and theta of direction vector
	float r = dir.Length();
	if (r == 0)	return Vector3(0, 0, 1);

	float oneOverR		= 1/r;
	float theta			= Math::ACos(dir.z * oneOverR);
	float phi			= atan2(dir.y, dir.x);
	float halfConeAngle = 0.5f*coneAngle;					// set up random vector within a cone that points upwards

	float phiGridSize	= Math::TWO_PI / (float)gridSizeX;
	float thetaGridSize	= halfConeAngle / (float)gridSizeY;

	float newPhi		= (xGridPos * phiGridSize)   + RandF(0, phiGridSize);		// a random point in gridcell [xGridPos][yGridPos]
	float newTheta		= (yGridPos * thetaGridSize) + RandF(0, thetaGridSize);
	float sa			= Math::Sin(newTheta);
	Vector3 result(Math::Cos(newPhi)*sa, Math::Sin(newPhi)*sa, Math::Cos(newTheta));

	// rotate the random vector towards the direction vector
	float ca	= Math::Cos(-theta);
	float sb	= Math::Sin(-phi);
	float cb	= Math::Cos(-phi);
	sa			= Math::Sin(-theta);

	Vector3 oldResult(result);
	result.z	= oldResult.x*sa + oldResult.z*ca;
	oldResult.x = oldResult.x*ca - oldResult.z*sa;
	result.x	= sb*oldResult.y + cb*oldResult.x;
	result.y	= cb*oldResult.y - sb*oldResult.x;

	return result;
}




Vector3 Random::RandomDirVec(const Vector3& dir, float coneAngle, float startPhi, float endPhi, float startTheta, float endTheta, bool midPoint)
{
	// determine r, phi and theta of direction vector
	float r = dir.Length();
	if (r == 0)	return Vector3(0, 0, 1);

	float oneOverR		= 1/r;
	float theta			= Math::ACos(dir.z * oneOverR);
	float phi			= atan2(dir.y, dir.x);
	//float halfConeAngle = 0.5f*coneAngle;					// set up random vector within a cone that points upwards

	float newPhi		= midPoint ? (endPhi+startPhi)*0.5f : RandF(startPhi, endPhi);
	float newTheta		= midPoint ? (endTheta+startTheta)*0.5f : RandF(startTheta*0.5f, endTheta*0.5f);
	float sa			= Math::Sin(newTheta);
	Vector3 result(Math::Cos(newPhi)*sa, Math::Sin(newPhi)*sa, Math::Cos(newTheta));

	// rotate the random vector towards the direction vector
	float ca	= Math::Cos(-theta);
	float sb	= Math::Sin(-phi);
	float cb	= Math::Cos(-phi);
	sa			= Math::Sin(-theta);

	Vector3 oldResult(result);
	result.z	= oldResult.x*sa + oldResult.z*ca;
	oldResult.x = oldResult.x*ca - oldResult.z*sa;
	result.x	= sb*oldResult.y + cb*oldResult.x;
	result.y	= cb*oldResult.y - sb*oldResult.x;

	return result;
}




Array<Vector3> Random::RandomDirVectorsStratisfied(const Vector3& dir, float coneAngle, int numVectors)
{
	Array<Vector3> result;

	int num = sqrtf((float)numVectors);

	for (int y=0; y<num; y++)
		for (int x=0; x<num; x++)
			result.Add( RandomDirVec(dir, coneAngle, num, num, x, y) );

	return result;
}



Array<Vector3> Random::RandomDirVectorsHammersley(const Vector3& dir, float coneAngle, int numVectors)
{
	Array<Vector3> arrayResult;
	
	// generate the uvset
	float *uvSet = new float[numVectors<<1];
	PlaneHammersley(uvSet, numVectors);

	int index = 0;
	for (int i=0; i<numVectors; i++)
	{
		// determine r, phi and theta of direction vector
		float r = dir.Length();
		assert(r!=0);

		float oneOverR		= 1.0f / r;
		float theta			= Math::ACos(dir.z * oneOverR);
		float phi			= atan2(dir.y, dir.x);
		float halfConeAngle = 0.5f*coneAngle;					// set up random vector within a cone that points upwards
		float newPhi		= uvSet[index++] * Math::TWO_PI;		// 0..2pi
		float newTheta		= uvSet[index++] * halfConeAngle;	// 0..halfConeAngle

		float sa			= Math::Sin(newTheta);
		Vector3 result(Math::Cos(newPhi)*sa, Math::Sin(newPhi)*sa, Math::Cos(newTheta));

		// rotate the random vector towards the direction vector
		float ca	= Math::Cos(-theta);
		float sb	= Math::Sin(-phi);
		float cb	= Math::Cos(-phi);
		sa			= Math::Sin(-theta);

		Vector3 oldResult(result);
		result.z	= oldResult.x*sa + oldResult.z*ca;
		oldResult.x = oldResult.x*ca - oldResult.z*sa;
		result.x	= sb*oldResult.y + cb*oldResult.x;
		result.y	= cb*oldResult.y - sb*oldResult.x;

		arrayResult.Add(result);
	}

	delete[] uvSet;

	return arrayResult;
}




Array<Vector3> Random::RandomDirVectorsHammersley2(const Vector3& dir, float coneAngle, int numVectors, int base)
{
	Array<Vector3> arrayResult;
	
	// generate the uvset
	float *uvSet = new float[numVectors<<1];
	PlaneHammersley2(uvSet, numVectors, base);

	int index = 0;
	for (int i=0; i<numVectors; i++)
	{
		// determine r, phi and theta of direction vector
		float r = dir.Length();
		assert(r!=0);

		float oneOverR		= 1.0f / r;
		float theta			= Math::ACos(dir.z * oneOverR);
		float phi			= atan2(dir.y, dir.x);
		float halfConeAngle = 0.5f*coneAngle;					// set up random vector within a cone that points upwards
		float newPhi		= uvSet[index++] * Math::TWO_PI;			// 0..2pi
		float newTheta		= uvSet[index++] * halfConeAngle;	// 0..halfConeAngle

		float sa			= Math::Sin(newTheta);
		Vector3 result(Math::Cos(newPhi)*sa, Math::Sin(newPhi)*sa, Math::Cos(newTheta));

		// rotate the random vector towards the direction vector
		float ca	= Math::Cos(-theta);
		float sb	= Math::Sin(-phi);
		float cb	= Math::Cos(-phi);
		sa			= Math::Sin(-theta);

		Vector3 oldResult(result);
		result.z	= oldResult.x*sa + oldResult.z*ca;
		oldResult.x = oldResult.x*ca - oldResult.z*sa;
		result.x	= sb*oldResult.y + cb*oldResult.x;
		result.y	= cb*oldResult.y - sb*oldResult.x;

		arrayResult.Add(result);
	}

	delete[] uvSet;

	return arrayResult;
}




Array<Vector3> Random::RandomDirVectorsHalton(const Vector3& dir, float coneAngle, int numVectors, int p2)
{
	Array<Vector3> arrayResult;
	
	// generate the uvset
	float *uvSet = new float[numVectors<<1];
	PlaneHalton(uvSet, numVectors, p2);

	int index = 0;
	for (int i=0; i<numVectors; i++)
	{
		// determine r, phi and theta of direction vector
		float r = dir.Length();
		assert(r!=0);

		float oneOverR		= 1.0f / r;
		float theta			= Math::ACos(dir.z * oneOverR);
		float phi			= atan2(dir.y, dir.x);
		float halfConeAngle = 0.5f*coneAngle;					// set up random vector within a cone that points upwards
		float newPhi		= uvSet[index++] * Math::TWO_PI;		// 0..2pi
		float newTheta		= uvSet[index++] * halfConeAngle;	// 0..halfConeAngle

		float sa			= Math::Sin(newTheta);
		Vector3 result(Math::Cos(newPhi)*sa, Math::Sin(newPhi)*sa, Math::Cos(newTheta));

		// rotate the random vector towards the direction vector
		float ca	= Math::Cos(-theta);
		float sb	= Math::Sin(-phi);
		float cb	= Math::Cos(-phi);
		sa			= Math::Sin(-theta);

		Vector3 oldResult(result);
		result.z	= oldResult.x*sa + oldResult.z*ca;
		oldResult.x = oldResult.x*ca - oldResult.z*sa;
		result.x	= sb*oldResult.y + cb*oldResult.x;
		result.y	= cb*oldResult.y - sb*oldResult.x;

		arrayResult.Add(result);
	}

	delete[] uvSet;

	return arrayResult;

}


Array<Vector3> Random::RandomDirVectorsHalton2(const Vector3& dir, float coneAngle, int numVectors, int baseA, int baseB)
{
	Array<Vector3> arrayResult;
	
	// generate the uvset
	float *uvSet = new float[numVectors<<1];
	PlaneHalton2(uvSet, numVectors, baseA, baseB);

	int index = 0;
	for (int i=0; i<numVectors; i++)
	{
		// determine r, phi and theta of direction vector
		float r = dir.Length();
		assert(r!=0);

		float oneOverR		= 1.0f / r;
		float theta			= Math::ACos(dir.z * oneOverR);
		float phi			= atan2(dir.y, dir.x);
		float halfConeAngle = 0.5f*coneAngle;					// set up random vector within a cone that points upwards
		float newPhi		= uvSet[index++] * Math::TWO_PI;			// 0..2pi
		float newTheta		= uvSet[index++] * halfConeAngle;	// 0..halfConeAngle

		float sa			= Math::Sin(newTheta);
		Vector3 result(Math::Cos(newPhi)*sa, Math::Sin(newPhi)*sa, Math::Cos(newTheta));

		// rotate the random vector towards the direction vector
		float ca	= Math::Cos(-theta);
		float sb	= Math::Sin(-phi);
		float cb	= Math::Cos(-phi);
		sa			= Math::Sin(-theta);

		Vector3 oldResult(result);
		result.z	= oldResult.x*sa + oldResult.z*ca;
		oldResult.x = oldResult.x*ca - oldResult.z*sa;
		result.x	= sb*oldResult.y + cb*oldResult.x;
		result.y	= cb*oldResult.y - sb*oldResult.x;

		arrayResult.Add(result);
	}

	delete[] uvSet;

	return arrayResult;
}




Vector3 Random::UVToVector(const Vector3& dir, float coneAngle, float u, float v)
{
	// determine r, phi and theta of direction vector
	float r = dir.Length();
	if (r == 0)	return Vector3(0, 0, 1);

	float oneOverR		= 1.0f / r;
	float theta			= Math::ACos(dir.z * oneOverR);
	float phi			= atan2(dir.y, dir.x);
	float halfConeAngle = 0.5f * coneAngle;					// set up random vector within a cone that points upwards
	float newPhi		= u * Math::TWO_PI;
	float newTheta		= v * halfConeAngle;
	float sa			= Math::Sin(newTheta);
	Vector3 result(Math::Cos(newPhi)*sa, Math::Sin(newPhi)*sa, Math::Cos(newTheta));

	// rotate the random vector towards the direction vector
	float ca	= Math::Cos(-theta);
	float sb	= Math::Sin(-phi);
	float cb	= Math::Cos(-phi);
	sa			= Math::Sin(-theta);

	Vector3 oldResult(result);
	result.z	= oldResult.x*sa + oldResult.z*ca;
	oldResult.x = oldResult.x*ca - oldResult.z*sa;
	result.x	= sb*oldResult.y + cb*oldResult.x;
	result.y	= cb*oldResult.y - sb*oldResult.x;

	return result;
}







/*
// Hammersley point sets. Deterministic and look random.
// Base p = 2, which is especially fast for computation.
void GenerateSphereHammersley(float *result, int n)
{
  float p, t, st, phi, phirad;
  int k, kk, pos;
  
  for (k=0, pos=0 ; k<n ; k++)
  {
    t = 0;
    for (p=0.5, kk=k ; kk ; p*=0.5, kk>>=1)
      if (kk & 1)                           // kk mod 2 == 1
	t += p;
    t = 2.0 * t  - 1.0;                     // map from [0,1] to [-1,1]

    phi = (k + 0.5) / n;                    // a slight shift
    phirad =  phi * 2.0 * MATH_PI;             // map to [0, 2 pi)

    st = Sqrt(1.0-t*t);
    result[pos++] = st * Cos(phirad);
    result[pos++] = st * Sin(phirad);
    result[pos++] = t;
  }
}



// Hammersley point sets for any base p1. Deterministic and look random.
void SphereHammersley2(float *result, int n, int p1)
{
  float a, p, ip, t, st, phi, phirad;
  int k, kk, pos;
  
  for (k=0, pos=0 ; k<n ; k++)
  {
    t = 0;
    ip = 1.0/p1;                           // recipical of p1
    for (p=ip, kk=k ; kk ; p*=ip, kk/=p1)  // kk = (int)(kk/p1)
      if ((a = kk % p1))
	t += a * p;
    t = 2.0 * t  - 1.0;                    // map from [0,1] to [-1,1]

    phi = (k + 0.5) / n;
    phirad =  phi * 2.0 * MATH_PI;            // map to [0, 2 pi)

    st = Sqrt(1.0-t*t);
    result[pos++] = st * Cos(phirad);
    result[pos++] = st * Sin(phirad);
    result[pos++] = t;
  }
}



// Halton point set generation
// two p-adic Van der Corport sequences
// Useful for incremental approach.
// p1 = 2, p2 = 3(default)
void SphereHalton(float *result, int n, int p2)
{
  float p, t, st, phi, phirad, ip;
  int k, kk, pos, a;
  
  for (k=0, pos=0 ; k<n ; k++)
  {
    t = 0;
    for (p=0.5, kk=k ; kk ; p*=0.5, kk>>=1)
      if (kk & 1)                          // kk mod 2 == 1
	t += p;
    t = 2.0 * t - 1.0;                     // map from [0,1] to [-1,1]
    st = Sqrt(1.0-t*t);

    phi = 0;
    ip = 1.0/p2;                           // recipical of p2
    for (p=ip, kk=k ; kk ; p*=ip, kk/=p2)  // kk = (int)(kk/p2)
      if ((a = kk % p2))
	phi += a * p;
    phirad =  phi * 4.0 * MATH_PI;            // map from [0,0.5] to [0, 2 pi)

    result[pos++] = st * Cos(phirad);
    result[pos++] = st * Sin(phirad);
    result[pos++] = t;
  }
}



// Halton point set generation
// two p-adic Van der Corport sequences
// Useful for incremental approach.
// p1 = 2(default), p2 = 3(default)
void SphereHalton2(float *result, int n, int p1, int p2)
{
  float p, t, st, phi, phirad, ip;
  int k, kk, pos, a;
  
  for (k=0, pos=0 ; k<n ; k++)
  {
    t = 0;
    ip = 1.0/p1;                           // recipical of p1
    for (p=ip, kk=k ; kk ; p*=ip, kk/=p1)  // kk = (int)(kk/p1)
      if ((a = kk % p1))
	t += a * p;
    t = 2.0 * t  - 1.0;                    // map from [0,1] to [-1,1]
    st = Sqrt(1.0-t*t);

    phi = 0;
    ip = 1.0/p2;                           // recipical of p2
    for (p=ip, kk=k ; kk ; p*=ip, kk/=p2)  // kk = (int)(kk/p2)
      if ((a = kk % p2))
	phi += a * p;
    phirad =  phi * 4.0 * MATH_PI;            // map from [0,0.5] to [0, 2 pi)

    result[pos++] = st * Cos(phirad);
    result[pos++] = st * Sin(phirad);
    result[pos++] = t;
  }
}
*/

// Hammersley point sets. Deterministic and look random.
// Base p = 2, which is especially fast for computation.
void Random::PlaneHammersley(float *result, int num)
{
	float p, u, v;
	int k, kk, pos;

	for (k=0, pos=0 ; k<num ; k++)
	{
		u = 0;
		for (p=0.5, kk=k; kk; p*=0.5, kk>>=1)
			if (kk & 1)                           // kk mod 2 == 1
				u += p;
	
		v = (k + 0.5) / num;

		result[pos++] = u;
		result[pos++] = v;
	}
}



// Hammersley point sets for any base p1. Deterministic and look random.
void Random::PlaneHammersley2(float *result, int num, int base)
{
	float a, p, ip, u, v;
	int k, kk, pos;

	for (k=0, pos=0; k<num; k++)
	{
		u  = 0;
		ip = 1.0 / base;                       // recipical of base
	
		for (p=ip, kk=k; kk; p*=ip, kk/=base)  // kk = (int)(kk/base)
			if ((a = kk % base))
				u += a * p;

		v = (k + 0.5) / num;

		result[pos++] = u;
		result[pos++] = v;
	}
}


// Halton point set generation
// two p-adic Van der Corport sequences
// Useful for incremental approach.
// p1 = 2, p2 = 3(default)
void Random::PlaneHalton(float *result, int num, int p2)
{
	float p, u, v, ip;
	int k, kk, pos, a;

	for (k=0, pos=0; k<num; k++)
	{
		u = 0;
		for (p=0.5, kk=k; kk; p*=0.5, kk>>=1)
			if (kk & 1)                          // kk mod 2 == 1
				u += p;

		v = 0;
		ip = 1.0 / p2;                           // recipical of p2
		for (p=ip, kk=k; kk; p*=ip, kk/=p2)  // kk = (int)(kk/p2)
		{
			a = kk % p2;
			if (a)
				v += a * p;
		}

		result[pos++] = u;
		result[pos++] = v;
	}
}


// Halton point set generation
// two p-adic Van der Corport sequences
// Useful for incremental approach.
// p1 = 2(default), p2 = 3(default)
void Random::PlaneHalton2(float *result, int num, int baseA, int baseB)
{
	float p, u, v, ip;
	int k, kk, pos, a;

	for (k=0, pos=0; k<num; k++)
	{
		u = 0;
		ip = 1.0/baseA;                           // recipical of baseA
		
		for (p=ip, kk=k ; kk ; p*=ip, kk/=baseA)  // kk = (int)(kk/baseA)
		{
			a = kk % baseA;
			if (a)
				u += a * p;
		}

		v = 0;
		ip = 1.0/baseB;                           // recipical of baseB
		for (p=ip, kk=k ; kk ; p*=ip, kk/=baseB)  // kk = (int)(kk/baseB)
		{
			a = kk % baseB;
			if (a)
				v += a * p;
		}

		result[pos++] = u;
		result[pos++] = v;
	}
}





//-----------------------------------------------

// constructor
HaltonSequence::HaltonSequence(int dimensions, int offset, int *primes)
{
	assert(dimensions > 0);

	mNextDim	= 0;
	mDimensions	= dimensions;
	mX			= new double[dimensions];
	mMemory		= sizeof(HaltonSequence) + dimensions * sizeof(double);

	mN			= offset;
	mN0			= offset;
	mRadical	= new double[dimensions];

	mOwnBase	= (!primes);   

	if (mOwnBase)
		mBase = FirstPrimes((int)dimensions);
	else
		mBase = primes;

	for(int j=0; j<dimensions; j++)
	{
		mRadical[j] = 1.0 / (double)mBase[j];
		mX[j] = 0.0;
	}

	SetInstance(mN0);
}




// destructor
HaltonSequence::~HaltonSequence()
{
	if (mOwnBase) 
		delete[] mBase; 

	mBase = NULL;
	delete[] mRadical; 
	mRadical = NULL;
}


// operator for the next value
void HaltonSequence::operator++()
{
	const double one = 1.0 - 1e-10;
	double h, hh, remainder;

	mN++;

	if (mN & 8191)
	{
		for(int j=0; j<mDimensions; j++)
		{
			remainder = one - mX[j];

			if (remainder < 0.0)
			{
				mX[j] = 0.0;
			}
			else 
			{
				if(mRadical[j] < remainder)
					mX[j] += mRadical[j];
				else
				{
					h = mRadical[j];
				
					do
					{
						hh = h;
						h *= mRadical[j];
					}
					while (h >= remainder);

					mX[j] += hh + h - 1.0;
				}	
			}
		}
	}
	else 
	{
		if (mN >= 1073741824) // 2^30
			SetInstance(0);
		else
			SetInstance(mN);
	}
}



// set the instance
void HaltonSequence::SetInstance(const int instance)
{
	unsigned long im;
	int           b;
	double        fac;

	mN = instance;
	for (int j=0; j<mDimensions; j++)
	{
		mX[j]	= 0.0;
		fac		= mRadical[j];
		b		= mBase[j];

		for(im=mN; im>0; im/=b, fac*=mRadical[j])
			mX[j] += fac * (double)(im % b);
	}
} 



// get the n first number of primes
int* HaltonSequence::FirstPrimes(const int n) const
{
	int *Prime, i, j, p, b;

	if (n == 0)
		return NULL;

	Prime = new int[n];
	assert(Prime);

	Prime[0] = 2;

	for (p=3, i=1; i<n; p+=2)
	{
		Prime[i] = p;

		// ORIGINAL
		//for(j=1; (b=Prime[j] <= p / Prime[j]) && (p % Prime[j]); j++);

		j = 1;
		b = (Prime[j] <= p / Prime[j]);
		while (b && (p % Prime[j]))
		{
			b = (Prime[j] <= p / Prime[j]);
			j++;
		}

		if (b == 0)
			i++;
	}

	return Prime;
}


}
