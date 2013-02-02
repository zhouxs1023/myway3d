/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKSolver.h"
#include "JacobianIKActorDOF.h"
#include "Node.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
JacobianIKSolver::JacobianIKSolver(JacobianIKData* ikData)
{
	mIKData			= ikData;
	mNumFunc		= NULL;
	mNumFuncC		= NULL;
	mFuncs			= NULL;
	mVars			= NULL;
	mTolerance		= 0.05f;
	mMaxIterations	= 200;
}


// destructor
JacobianIKSolver::~JacobianIKSolver()
{
}


// calculates and returns in the array the IK solution
void JacobianIKSolver::CalcIK(Array<float>& solution)
{
	// retrieve the current number of DOFs
	mNumDOFs = mIKData->GetNumDOF();

	int n, i;

	Array<float> orig;

	mVars = new bool[mNumDOFs];
	mNumFuncC = new int[mIKData->GetNumConstraints()];


	// original values from DOFs
	mIKData->GetDOFValues(orig);

	// initialization for solution array // copy again, needed?...(Benny)
	mSolution = orig;

	// mVars construction
	CVars();

	// getting the number of functions.
	CNumFunc();

	// if no any constraint must be applied
	if (mNumFunc == 0)
		return;

	// get which constraint is each function from
	CFuncs();

	// variables number
	mNumVar = 0;
	for (n = 0; n < mNumDOFs; n++)
		mNumVar += mVars[n];
	
	// matrices for partial solutions
	mxSol.SetSize(mNumVar, 1);
	mxSolP.SetSize(1,2);

	// insert in mxSol the original values for implied variables
	i = 0;
	for (n=0; n<mNumDOFs; n++)
	{
		if (mVars[n] == 1)
		{
			mxSol(i,0) = orig[n];
			i++;
		}
	}

	// functions initial and final values
	CPositions();


	// solve with actual settings
	Solve();

	// If method doesn't coverge or any singularity is found tryes to solve 
	// again. This time it uses the variables values in previous frame in which
	// the result may be similar.
	if (mxSol.GetNumRows() == 0&& solution.Size() == mNumDOFs)
	{
		mxSol.SetSize(mNumVar, 1);
		i = 0;
		for (n = 0; n < mNumDOFs; n++)
		{	
			if (mVars[n] == 1)
			{
				mxSol(i,0) = solution[n];
				i++;
			}
		}
		CPositions();

		Solve();
	}

	// introducing results in DOFs array
	if (mxSol.GetNumRows() != 0)
	{
		i = 0;
		for (n = 0; n < mNumDOFs; n++)
		{
			if (mVars[n] == 1)
			{
				mSolution[n] = mxSol(i,0);
				i++;
			}
		}
	}

	solution = mSolution;

	delete mNumFuncC;
	mNumFuncC = NULL;
	
	delete mVars;
	mVars = NULL;
}


//
void JacobianIKSolver::Solve()
{
	NMatrix xJacob(mNumFunc, mNumVar);	// jacobian matrix
	NMatrix xPsI(mNumFunc, mNumVar);	// jacobian pseudo-inverse natrix
	NMatrix xVel(mNumFunc, 1);			// function delta matrix
	NMatrix xI;
	xI.Unit(mNumVar);					// unit matrix
	NMatrix xAux, xAuxSol, xAuxSol2;

	int n, m, i, j;
	bool ifc = false;

	float k1 = 0.04f;

	// if the difference between initial and final values is less than the tolerance we don`t have to solve anything
	for (i=0; i<mNumFunc; i++)
	{
		if (fabs(mIPosition[i] - mFPosition[i]) > mTolerance)
		{
			ifc = true;
			break;
		}
	}

	// Function delta matrix
	for (n=0; n<mNumFunc; n++)
		xVel(n,0) = k1 * (mFPosition[n] - mIPosition[n]);

	// number of done iterations
	n = 0;

	//  While result has not been reached and the number of iteration is less
	// than maximun.
	while (ifc)
	{
		// Gets jacobian matrix.
		Jacobian(xJacob);

		// Gets the pseudoinverse of jacobian matrix
		xPsI = PseudoInverse(xJacob);

		// Next variables values.
		xAuxSol = mxSol + xPsI * xVel;

		// Test DOFs limits
		Array<int> nl = TestLimits(xAuxSol);

		//  Reevaluation of next variables values without considering
		// those which are out of range in previous evaluation.
		m = 0;
		while (nl.Size() != 0&& m < mNumVar)
		{
			xAuxSol = mxSol;
			xAux = xJacob;

			//  Remove the variables columns in jacobian and the rows in
			// the variables vector. 
			for (i = nl.Size() - 1; i <= 0; i++)
			{
				xAuxSol.RemoveRow(nl[i]);
				xAux.RemoveCol(nl[i]);
			}
			
			// New variables values.
			xAuxSol = xAuxSol + PseudoInverse(xAux) * xVel;

			// Reconstructing variables vector.
			j = 0;
			xAuxSol2 = xAuxSol;
			xAuxSol.SetSize(mNumVar, 1);
			for (i=0; i<mNumVar; i++)
			{
				if (nl[j] == i)
				{
					xAuxSol(i, 0) = mxSol(i, 0);
					j++;
				}
				else
					xAuxSol(i, 0) = xAuxSol2(i - j, 0);
			}

			// test limits in new variables vector
			nl = TestLimits(xAuxSol);
			m++;
		}


		mxSol = xAuxSol;

		// apply it to the actor instance
		SetValues();

		// initial function values for the next step
		mIPosition.Clear();
		for (i=0; i<mIKData->GetNumConstraints(); i++)
			mIKData->GetConstraint(i)->GetResults(mIPosition);

		// function delta for the next step
		for (i = 0; i < mNumFunc; i++)
			xVel(i, 0) = k1 * (mFPosition[i] - mIPosition[i]);

		// one more done
		n++;

		// if the difference is less than the tolerance or the MAX_STEPS is reached
		// stops iteration (ifc = 0)
		// if the result is NAN or MAX_STEP is reached without having a solution
		// an empty matrix is returned to force a new resolution
		ifc = false;
		for (i=0; i<mNumFunc; i++)
		{
			if (fabs(mIPosition[i] - mFPosition[i]) > mTolerance)
			{
				ifc = true;
				break;
			}

			if (!_finite(mIPosition[i]))
			{
				ifc = false;
				mxSol.SetSize(0, 0);
				break;
			}
		}
		if (n>mMaxIterations || m>mNumVar)
		{
			ifc = false;
			mxSol.SetSize(0, 0);
		}
	}
}


//
void JacobianIKSolver::CFuncs()
{
	if (mFuncs)
		delete mFuncs;

	mFuncs = new int[mNumFunc];

	// get the number of constraints from the ik data
	const int numConstraints = mIKData->GetNumConstraints();

	int j = 0;
	for (int n=0; n<numConstraints; n++) 
	{
		for (int i=0; i<mNumFuncC[n]; i++)
		{
			mFuncs[j] = n;
			j++;
		}
	}
}


//
void JacobianIKSolver::CVars()
{
	// get the number of constraints from the ik data
	const int numConstraints = mIKData->GetNumConstraints();

	int i;

	// initialize all flags to false
	for (i=0; i<mNumDOFs; i++)
		mVars[i] = false;

	for (i=0; i<numConstraints; i++)
	{
		// retrieve the current node from the ik data
		Node* node = mIKData->GetConstraintNode(i);

		for (int m=0; m<mNumDOFs; m++)
			mVars[m] = mVars[m] || mIKData->GetInfluence(node, m);
	}

	// set all DOFs to false over base nodes
	for (i=0; i<numConstraints; i++)
	{
		// retrieve the current base node of the constraint from the ik data
		Node* node = mIKData->GetConstraint(i)->GetBaseNode();

		while (node->GetParent() != NULL)
		{
			int d1 = mIKData->GetNode1stDOF(node);

			// get the number of DOFs from the ik data
			const int numDOFs = mIKData->GetNumDOF(node);
	
			for (int m=0; m<numDOFs; m++)
				mVars[d1 + m] = false;

			// set the node to the node's parent and go again
			node = node->GetParent();
		}
	}
}


//
void JacobianIKSolver::CNumFunc()
{
	mNumFunc = 0;

	for (int i=0; i<mIKData->GetNumConstraints(); i++)
	{
		mNumFuncC[i] = mIKData->GetConstraint(i)->GetNumFunc();
		mNumFunc = mNumFunc + mNumFuncC[i];
	}
}


//
void JacobianIKSolver::CPositions()
{
	SetValues();

	mIPosition.Clear();
	mFPosition.Clear();

	SetValues(); // why twice ? (Benny)

	for (int i=0; i<mIKData->GetNumConstraints(); i++)
	{
		// retrieve the current constraint from the ik data
		JacobianIKConstraint* constraint = mIKData->GetConstraint(i);

		// get the results from the constraint
		constraint->GetResults(mIPosition);

		// get the values from the constraint
		constraint->GetValues(mFPosition);
	}
}


//
void JacobianIKSolver::SetValues()
{
	// if those are the same then the values have been already introduced
	if (mxSol == mxSolP)
		return;

	Array<float> values = mSolution;

	// replace the values changed by the solver
	int i=0;
	for (int n=0; n<mNumDOFs; n++)
	{
		if (mVars[n] == 1)
		{
			values[n] = mxSol(i,0);
			i++;
		}
	}

	mIKData->SetValues(values);

	mxSolP = mxSol;
}


//
void JacobianIKSolver::Jacobian(NMatrix& jacob)
{
	// get the number of constraints from the ik data
	const int numConstraints = mIKData->GetNumConstraints();

	NMatrix grad;

	SetValues();

	int j	= 0;
	int row	= 0;
	for (int n=0; n<numConstraints; n++)
	{
		// adding each gradient row
		mIKData->GetConstraint(n)->CalcGradient(grad);

		for (int i=0; i<grad.GetNumRows(); i++)
		{
			j = 0;

			for (int m=0; m<mNumDOFs; m++)
			{
				if (mVars[m])
				{
					jacob(row,j) = grad(i,m);
					j++;
				}
			}
			row++;
		}
	}
}


//
Array<int> JacobianIKSolver::TestLimits(NMatrix& matrix)
{
	Array<int> nl;

	int j=0;
	for (int i=0; i<mNumDOFs; i++)
	{
		if (mVars[i])
		{
			float aux = matrix(j,0);
			float max = mIKData->GetMax(i);
			float min = mIKData->GetMin(i);

			if (max != min&& (aux > max || aux < min))
				nl.Add(j);

			j++;
		}
	}

	return nl;
}

}