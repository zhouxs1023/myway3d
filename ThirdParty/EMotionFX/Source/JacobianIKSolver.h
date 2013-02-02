/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __JACOBIANIKSOLVER_H__
#define __JACOBIANIKSOLVER_H__

// include the Core system
#include "../Core/Source/MCore.h"

#include "JacobianIKData.h"

namespace EMotionFX
{

// forward declarations
class Actor;

/**
 * The Jacobian Pseudo Inverse IK solver.
 * This solver is not really realtime, but more interactive.
 * It is a lot slower than the CCD solver and will mostly be used for non-game applications.
 */
class JacobianIKSolver
{
	DECLARE_CLASS(JacobianIKSolver)

	public:
		/**
		 * Constructor.
		 * @param ikData Pointer to the jacobian ik data to use.
		 **/
		JacobianIKSolver(JacobianIKData* ikData);

		/**
		 * Destructor.
		 */
		~JacobianIKSolver();

		/**
		 * Set the maximun difference between constraints functions and results.
		 * @param tolerance The tolerance value.
		 */
		inline void SetTolerance(const float tolerance);

		/**
		 * Calculate the ik solution and store it in the given array.
		 * @param solution Array of floats in which the solution gets stored.
		 */
		void CalcIK(MCore::Array<float>& solution);

	private:
		/**
		 * Solve iterations for current state.
		 **/
		void Solve();

		/**
		 * Obtain Jacobian matrix for constraints functions from DOF values in mxSol.
		 * @param jacob
		 **/
		void Jacobian(MCore::NMatrix& jacob);

		/**
		 * Apply actual DOF values over the actor instance.
		 */ 
		void SetValues();

		/**
		 * Evaluate which DOFs are variables in solving.
		 **/
		void CVars();

		/**
		 * Evaluate the number of functions for each constraint.
		 **/
		void CNumFunc();

		/**
		 * Construct the member function array.
		 **/
		void CFuncs();

		/**
		 * Evaluate the constraint functions, there use to be positions.
		 **/
		void CPositions();

		/**
		 * Evaluate which variables are over limits.
		 **/
		MCore::Array<int> TestLimits(MCore::NMatrix& matrix);

		MCore::NMatrix		mxSolP;				/**< Last solution values evaluated by constraint functions */
		MCore::NMatrix		mxSol;				/**< Temporal DOFs values for desired values of constraints */
		MCore::Array<float>	mIPosition;			/**< Starting values for the constraints functions */
		MCore::Array<float>	mFPosition;			/**< Desired values for constraints functions */
		MCore::Array<float>	mSolution;			/**< Solution for all the DOFs */
		JacobianIKData*		mIKData;			/**< Group of constraints to solve */
		float				mTolerance;			/**< Maximun allowed difference between desired values and obtained ones */
		int					mNumVar;			/**< Number of DOFs IKSolver can adjust. It isn't the number of DOF of the actor. Not all the DOFs affect the kinematic.*/
		int					mNumDOFs;			/**< Number of DOFs of the actor */
		int					mNumFunc;			/**< Number of functions the constraints are applied through */
		int					mMaxIterations;		/**< The maximum number of iterations (tradeoff between speed and accuracy) [default value=200]. */
		int*				mNumFuncC;			/**< Number of functions introduced for each constraint */
		int*				mFuncs;				/**< Constraint which introduces each function */
		bool*				mVars;				/**< mVars[i] = true if the i DOF is implied in IK */
};

// include inline code
#include "JacobianIKSolver.inl"

} // namespace EMotionFX


#endif