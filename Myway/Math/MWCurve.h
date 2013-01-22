#pragma once

#include "MWVector2.h"
#include "MWVector3.h"

namespace Myway {

	template <class T>
	class Curve
	{
		DECLARE_ALLOC();

	public:
		Curve() 
		{
			mNumPoints = 100;
			mPoints = new T[mNumPoints];
		}

		Curve(int count)
		{
			mNumPoints = count;
			mPoints = new T[mNumPoints];
		}

		~Curve()
		{
			safe_delete_array (mPoints);
		}

		void SetPointCount(int count)
		{
			d_assert (count > 0);

			if (mNumPoints != count)
			{
				mNumPoints = count;
				safe_delete_array (mPoints);

				mPoints = new T[count];
			}
		}

		int GetPointCount() const
		{ 
			return mNumPoints;
		}

		
		void SetPoint(int index, const T & p)
		{
			d_assert (index < mNumPoints);
			
			mPoints[index] = p;
		}

		const T & GetPoint(int index)
		{
			d_assert (index < mNumPoints);

			return mPoints[index];
		}

		void Smooth(int step)
		{
			T * points = new T[mNumPoints];

			for (int i = 0; i < mNumPoints; ++i)
			{
				int count = 1;
				T sum = mPoints[i];

				int start = (i - step > 0) ? i - step : 0;
				int end = (i + step < mNumPoints) ? i + step : mNumPoints;

				for (int j = start; j < i; ++j)
					sum += mPoints[j], count++;

				for (int j = i + 1; j < end; ++j)
					sum += mPoints[j], count++;

				points[i] = sum / count;
			}

			for (int i = 0; i < mNumPoints; ++i)
				mPoints[i] = points[i];

			delete points;
		}

	protected:
		Curve(const Curve & rk)
		{
			mPoints = NULL;

			*this = rk;
		}

		Curve & operator = (const Curve & rk)
		{
			d_assert (this != &rk);

			safe_delete_array (mPoints);

			mPoints = new T[rk.mNumPoints];

			for (int i = 0; i < mNumPoints; ++i)
				mPoints[i] = rk.mPoints[i];
		}

	protected:
		int mNumPoints;
		T * mPoints;
	};

	typedef Curve<Vec3> Curve3;
	typedef Curve<Vec2> Curve2;


	template <class T>
	class BezierCurve
	{
	public:
		BezierCurve()
		{
			mControl[0] = 0;
			mControl[1] = 0;
			mControl[2] = 0;
			mControl[3] = 0;
		}

		~BezierCurve()
		{
		}

		void SetControl(int index, const T & control)
		{
			mControl[index] = control;
		}

		const T & GetControl(int index) const
		{
			d_assert (index >= 0 && index < 4);

			return mControl[index];
		}

		T GetPoint(float t)
		{
			if (t < 0)
				t = 0;
			else if (t > 1)
				t = 1;

			mPoints[i] = mControl[0] * (1 - t) * (1 - t) * (1 - t) + 
				mControl[1] * (1 - t) * (1 - t) * t +
				mControl[2] * (1 - t) * t * t +
				mControl[3] * t * t * t;
		}

		void ToCurve(Curve<T> & curve)
		{
			int numPoints = curve.GetPointCount();

			for (int i = 0; i < numPoints; ++i)
			{
				float t = i / (float)(numPoints - 1);

				T p = mControl[0] * (1 - t) * (1 - t) * (1 - t) + 
					mControl[1] * (1 - t) * (1 - t) * t +
					mControl[2] * (1 - t) * t * t +
					mControl[3] * t * t * t;

				curve.SetPoint(i, p);
			}
		}

	protected:
		T mControl[4];
	};

	typedef BezierCurve<Vec3> BezierCurve3;
	typedef BezierCurve<Vec2> BezierCurve2;
}