/*
* EMotion FX 2 - Character Animation System
* Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
* All Rights Reserved.
*/

#ifndef __ROOT_SOLVER_H
#define __ROOT_SOLVER_H

// include the required files
#include "Macros.h"
#include "SourceDeclaration.h"


namespace MCore
{

/**
 * The class containing methods to solve quadric, cubic and quartic equations.
 */
class RootSolver
{
	public:
		/**
		 * Solve a quadric equation.
		 * @param c The coefficients.
		 * @param s The values that will contain the solutions (output).
		 * @result The number of roots.
		 */
		static int SolveQuadric(float c[3], float s[2]);

		/**
		 * Solve a cubic equation.
		 * @param c The coefficients.
		 * @param s The values that will contain the solutions (output).
		 * @result The number of roots.
		 */
		static int SolveCubic(float c[4], float s[3]);

		/**
		 * Solve a quartic equation.
		 * @param c The coefficients.
		 * @param s The values that will contain the solutions (output).
		 * @result The number of roots.
		 */
		static int SolveQuartic(float c[5], float s[4]);
};


} // namespace MCore

#endif