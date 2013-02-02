/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKSolver.h"


// set the tolerance of the ik solver
inline void JacobianIKSolver::SetTolerance(const float tolerance)
{
	mTolerance = tolerance;
}