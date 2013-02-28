#include "NewtonJoint.h"
#include "NewtonWorld.h"
#include "NewtonBody.h"
#include "NewtonShape.h"

namespace Myway {

	namespace Newton {

		tJoint::tJoint()
			: mJoint(NULL)
			, mUserData(NULL)
		{
		}

		tJoint::~tJoint()
		{
			if(mJoint)
			{
				if (NewtonJointGetUserData(mJoint))
				{
					NewtonJointSetDestructor(mJoint, NULL);
					NewtonDestroyJoint(tWorld::Instance()->_getNewtonWorld(), mJoint);
				}
			}
		}

		void tJoint::SetCollisionState(int state)
		{
			NewtonJointSetCollisionState(mJoint, state);
		}

		int tJoint::GetCollisionState() const
		{
			return NewtonJointGetCollisionState(mJoint);
		}

		void tJoint::SetStiffness(float v)
		{
			NewtonJointSetStiffness(mJoint, v);
		}

		float tJoint::GetStiffness() const
		{
			return NewtonJointGetStiffness(mJoint);
		}








		tBall::tBall(const tBody * child, const tBody * parent, const Vec3 & pos)
		{
			NewtonWorld * tworld = tWorld::Instance()->_getNewtonWorld();
			NewtonBody * tbody0 = child->_getNewtonBody();
			NewtonBody * tbody1 = parent ? parent->_getNewtonBody() : NULL;

			mJoint = NewtonConstraintCreateBall(tworld, &pos.x, tbody0, tbody1);

			d_assert (mJoint);

			NewtonJointSetUserData(mJoint, this);
		}

		tBall::~tBall()
		{
		}

		Vec3 tBall::GetAngle() const
		{
			Vec3 angle;

			NewtonBallGetJointAngle(mJoint, &angle.x);

			return angle;
		}

		Vec3 tBall::GetOmega() const
		{
			Vec3 omega;

			NewtonBallGetJointOmega(mJoint, &omega.x);

			return omega;
		}

		Vec3 tBall::GetForce() const
		{
			Vec3 force;

			NewtonBallGetJointForce(mJoint, &force.x);

			return force;
		}

		void tBall::SetLimits(const Vec3 & pin, float maxCone, float maxTwist)
		{
			NewtonBallSetConeLimits(mJoint, &pin.x, maxCone, maxTwist);
		}






		unsigned tHinge::NewtonCallback(const NewtonJoint* hinge, NewtonHingeSliderUpdateDesc* desc)
		{
			tHinge * me = (tHinge *)hinge;

			me->retVal = 0;
			me->desc = desc;

			me->OnCallback(NULL, NULL);

			return me->retVal;
		}

		tHinge::tHinge(const tBody * child, const tBody * parent, const Vec3 & pos, const Vec3 & pin)
			: desc(NULL)
			, retVal(0)
		{
			NewtonWorld * tworld = tWorld::Instance()->_getNewtonWorld();
			NewtonBody * tbody0 = child->_getNewtonBody();
			NewtonBody * tbody1 = parent ? parent->_getNewtonBody() : NULL;

			mJoint = NewtonConstraintCreateHinge(tworld, &pos.x, &pin.x, tbody0, tbody1);

			d_assert (mJoint);

			NewtonJointSetUserData(mJoint, this);
			NewtonHingeSetUserCallback(mJoint, tHinge::NewtonCallback);
		}

		tHinge::~tHinge()
		{

		}

		float tHinge::GetAngle() const
		{
			return NewtonHingeGetJointAngle(mJoint);
		}

		float tHinge::GetOmega() const
		{
			return NewtonHingeGetJointOmega(mJoint);
		}

		Vec3 tHinge::GetForce() const
		{
			Vec3 force;

			NewtonHingeGetJointForce(mJoint, &force.x);

			return force;
		}

		void tHinge::SetCallbackAccel(float accel)
		{
			if (desc)
			{
				desc->m_accel = accel;
				retVal = 1;
			}
		}

		void tHinge::SetCallbackFrictionMin(float fmin)
		{
			if (desc)
			{
				desc->m_minFriction = fmin;
				retVal = 1;
			}
		}

		void tHinge::SetCallbackFrictionMax(float fmax)
		{
			if (desc)
			{
				desc->m_maxFriction = fmax;
				retVal = 1;
			}
		}

		float tHinge::GetCallbackTimeStep() const
		{
			return desc ? desc->m_timestep : 0;
		}

		float tHinge::CalcuStopAlpha(float rads) const
		{
			return desc ? NewtonHingeCalculateStopAlpha(mJoint, desc, rads) : 0;
		}





		unsigned tSlider::NewtonCallback(const NewtonJoint* slider, NewtonHingeSliderUpdateDesc* desc)
		{
			tSlider * me = (tSlider *)slider;

			me->retVal = 0;
			me->desc = desc;

			me->OnCallback(NULL, NULL);

			return me->retVal;
		}


		tSlider::tSlider(const tBody * child, const tBody * parent, const Vec3 & pos, const Vec3 & pin)
			: desc(NULL)
			, retVal(0)
		{
			NewtonWorld * tworld = tWorld::Instance()->_getNewtonWorld();
			NewtonBody * tbody0 = child->_getNewtonBody();
			NewtonBody * tbody1 = parent ? parent->_getNewtonBody() : NULL;

			mJoint = NewtonConstraintCreateSlider(tworld, &pos.x, &pin.x, tbody0, tbody1);

			d_assert (mJoint);

			NewtonJointSetUserData(mJoint, this);
			NewtonHingeSetUserCallback(mJoint, tSlider::NewtonCallback);
		}

		tSlider::~tSlider()
		{
		}

		float tSlider::GetPosit() const
		{
			return NewtonSliderGetJointPosit(mJoint);
		}

		float tSlider::GetVeloc() const
		{
			return NewtonSliderGetJointVeloc(mJoint);
		}

		Vec3 tSlider::GetForce() const
		{
			Vec3 force;

			NewtonSliderGetJointForce(mJoint, &force.x);

			return force;
		}

		void tSlider::SetCallbackAccel(float accel)
		{
			if (desc)
			{
				desc->m_accel = accel;
				retVal = 1;
			}
		}

		void tSlider::SetCallbackFrictionMin(float fmin)
		{
			if (desc)
			{
				desc->m_minFriction = fmin;
				retVal = 1;
			}
		}

		void tSlider::SetCallbackFrictionMax(float fmax)
		{
			if (desc)
			{
				desc->m_maxFriction = fmax;
				retVal = 1;
			}
		}

		float tSlider::GetCallbackTimeStep() const
		{
			return desc ? desc->m_timestep : 0;
		}

		float tSlider::CalcuStopAccel(float dist) const
		{
			return desc ? NewtonSliderCalculateStopAccel(mJoint, desc, dist) : 0;
		}








		unsigned tUniversal::NewtonCallback(const NewtonJoint* universal, NewtonHingeSliderUpdateDesc* desc)
		{
			tUniversal * me = (tUniversal *)universal;

			me->retVal = 0;
			me->desc = desc;

			me->OnCallback(NULL, NULL);

			return me->retVal;
		}

		tUniversal::tUniversal(const tBody * child, const tBody * parent, const Vec3 & pos, const Vec3 & pin, const Vec3 & pin1)
		{
			NewtonWorld * tworld = tWorld::Instance()->_getNewtonWorld();
			NewtonBody * tbody0 = child->_getNewtonBody();
			NewtonBody * tbody1 = parent ? parent->_getNewtonBody() : NULL;

			mJoint = NewtonConstraintCreateUniversal(tworld, &pos.x, &pin.x, &pin1.x, tbody0, tbody1);

			d_assert (mJoint);

			NewtonJointSetUserData(mJoint, this);
			NewtonUniversalSetUserCallback(mJoint, tUniversal::NewtonCallback);
		}

		tUniversal::~tUniversal()
		{
		}

		float tUniversal::GetAngle0() const
		{
			return NewtonUniversalGetJointAngle0(mJoint);
		}

		float tUniversal::GetAngle1() const
		{
			return NewtonUniversalGetJointAngle1(mJoint);
		}

		float tUniversal::GetOmega0() const
		{
			return NewtonUniversalGetJointOmega0(mJoint);
		}

		float tUniversal::GetOmega1() const
		{
			return NewtonUniversalGetJointOmega1(mJoint);
		}

		Vec3 tUniversal::GetForce() const
		{
			Vec3 force;

			NewtonUniversalGetJointForce(mJoint, &force.x);

			return force;
		}

		void tUniversal::SetCallbackAccel(float accel, unsigned axis)
		{
			if (axis > 1)
				return ;

			if (desc)
			{
				retVal |= axis;
				desc[axis].m_accel = accel;
			}
		}

		void tUniversal::SetCallbackFrictionMin(float fmin, unsigned axis)
		{
			if (axis > 1)
				return ;

			if (desc)
			{
				retVal |= axis;
				desc[axis].m_minFriction = fmin;
			}
		}

		void tUniversal::SetCallbackFrictionMax(float fmax, unsigned axis)
		{
			if (axis > 1)
				return ;

			if (desc)
			{
				retVal |= axis;
				desc[axis].m_maxFriction = fmax;
			}
		}

		float tUniversal::GetCallbackTimeStep() const
		{
			return desc ? desc->m_timestep : 0;
		}

		float tUniversal::CalcuStopAlpha0(float angle) const
		{
			return desc ? NewtonUniversalCalculateStopAlpha0(mJoint, desc, angle) : 0;
		}

		float tUniversal::CalcuStopAlpha1(float angle) const
		{
			return desc ? NewtonUniversalCalculateStopAlpha1(mJoint, desc, angle) : 0;
		}









		tUpVector::tUpVector(const tBody * body, const Vec3 & pin)
		{
			NewtonWorld * tworld = tWorld::Instance()->_getNewtonWorld();
			NewtonBody * tbody0 = body->_getNewtonBody();

			mJoint = NewtonConstraintCreateUpVector(tworld, &pin.x, tbody0);

			NewtonJointSetUserData(mJoint, this);
		}

		tUpVector::~tUpVector()
		{
		}

		void tUpVector::SetPin(const Vec3 & pin)
		{
			NewtonUpVectorSetPin(mJoint, &pin.x);
		}

		Vec3 tUpVector::GetPin() const
		{
			Vec3 pin;

			NewtonUpVectorGetPin(mJoint, &pin.x);

			return pin;
		}
	}

}