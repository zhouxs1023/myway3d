//////////////////////////////////////////////////////////////////////////
//
// Newton Integration.
//   newton physics engine: http://newtondynamics.com/forum/newton.php
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "Newton.h"
#include "Myway.h"

namespace Myway {

	namespace Newton {

		class tWorld;
		class tBody;

		class tJoint
		{
			DECLARE_ALLOC();

		public:
			tJoint();
			virtual ~tJoint();

			// 1 : on, 0 : off
			void SetCollisionState(int state);
			int GetCollisionState() const;

			void SetStiffness(float v);
			float GetStiffness() const;

			void SetUserData(void * data) { mUserData = data; }
			void * GetUserData() const { return mUserData; }

		protected:
			NewtonJoint * mJoint;
			tWorld * mWorld;

			void * mUserData;
		};



		class tBall : public tJoint
		{
		public:
			tBall(const tBody * child, const tBody * parent, const Vec3 & pos);
			~tBall();

			Vec3 GetAngle() const;
			Vec3 GetOmega() const;
			Vec3 GetForce() const;

			void SetLimits(const Vec3 & pin, float maxCone, float maxTwist);
		};




		class tHinge : public tJoint
		{
			static unsigned NewtonCallback(const NewtonJoint* hinge, NewtonHingeSliderUpdateDesc* desc);

		public:
			Event OnCallback;

		public:
			tHinge(const tBody * child, const tBody * parent, const Vec3 & pos, const Vec3 & pin);
			~tHinge();

			float GetAngle() const;
			float GetOmega() const;
			Vec3 GetForce() const;

			void SetCallbackAccel(float accel);
			void SetCallbackFrictionMin(float fmin);
			void SetCallbackFrictionMax(float fmax);

			float GetCallbackTimeStep() const;

			float CalcuStopAlpha(float rads) const;

		protected:
			NewtonHingeSliderUpdateDesc * desc;
			int retVal;
		};








		class tSlider : public tJoint
		{
			static unsigned NewtonCallback(const NewtonJoint* slider, NewtonHingeSliderUpdateDesc* desc);

		public:
			Event OnCallback;

		public:
			tSlider(const tBody * child, const tBody * parent, const Vec3 & pos, const Vec3 & pin);
			~tSlider();

			float GetPosit() const;
			float GetVeloc() const;
			Vec3 GetForce() const;

			void SetCallbackAccel(float accel);
			void SetCallbackFrictionMin(float fmin);
			void SetCallbackFrictionMax(float fmax);
			float GetCallbackTimeStep() const;

			float CalcuStopAccel(float dist) const;

			float CalcuStopAlpha(float angle) const;

		protected:
			NewtonHingeSliderUpdateDesc * desc;
			int retVal;
		};







		class tUniversal : public tJoint
		{
			static unsigned NewtonCallback(const NewtonJoint* universal, NewtonHingeSliderUpdateDesc* desc);

		public:
			Event OnCallback;

		public:
			tUniversal(const tBody * child, const tBody * parent, const Vec3 & pos, const Vec3 & pin, const Vec3 & pin1);
			~tUniversal();

			float GetAngle0() const;
			float GetAngle1() const;

			float GetOmega0() const;
			float GetOmega1() const;

			Vec3 GetForce() const;

			void SetCallbackAccel(float accel, unsigned axis);
			void SetCallbackFrictionMin(float fmin, unsigned axis);
			void SetCallbackFrictionMax(float fmax, unsigned axis);
			float GetCallbackTimeStep() const;

			float CalcuStopAlpha0(float angle) const;
			float CalcuStopAlpha1(float angle) const;

		protected:
			NewtonHingeSliderUpdateDesc * desc;
			int retVal;
		};





		/*class tUserJoint : public tJoint
		{
		public:
			tUserJoint(unsigned int maxDOF, const tBody * body0, const tBody * body1);
		};*/


	}
}